#include "ssh_clipboard/event_loop/mac_event_loop.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <system_error>
#include <cstring>
#include <thread>
#include <cstring>
#include <cerrno>

namespace {
  using namespace ssh_clipboard::event_loop;
  bool simulate_kqueue_failure = false;
bool simulate_kevent_failure = false;
int simulated_poll_fd = 42;
bool close_called = false;

class MockKevent {
public:
  MOCK_METHOD6(kevent,
               int(int fd, const struct kevent* changelist, int nchanges,
                   struct kevent* eventlist, int nevents,
                   const struct timespec* timeout));
};
MockKevent* g_mockKevent = nullptr;

// Our fake kqueue: if we simulate failure, return -1; otherwise return a valid poll_fd.
int kqueue() {
  if (simulate_kqueue_failure) {
    errno = EMFILE;
    return -1;
  }
  return simulated_poll_fd;
}

// Our fake kevent. (The event loop code uses kevent both to add/remove fds and to poll.)
int kevent(int fd, const struct kevent* changelist, int nchanges,
           struct kevent* eventlist, int nevents,
           const struct timespec* timeout) {
  // If a mock is installed, delegate to it.
  if (g_mockKevent)
    return g_mockKevent->kevent(fd, changelist, nchanges, eventlist, nevents, timeout);

  if (simulate_kevent_failure) {
    errno = EFAULT;
    return -1;
  }
  // When adding or removing an fd (changelist is not null) just return success.
  if (changelist != nullptr && nchanges == 1)
    return 0;
  // When polling (eventlist is not null), simulate one event.
  if (eventlist != nullptr && nevents > 0) {
    // Our fake returns an event for fd 100.
    eventlist[0].ident = 100;
    return 1;
  }
  return 0;
}

// Our fake close. We simply record that it was called.
int close(int fd) {
  close_called = true;
  return 0;
}

// Helper to reset our globals between tests.
void resetTestGlobals() {
  simulate_kqueue_failure = false;
  simulate_kevent_failure = false;
  simulated_poll_fd = 42;
  close_called = false;
  g_mockKevent = nullptr;
}

TEST(MacEventLoopTest, ConstructorThrowsOnKqueueFailure) {
  resetTestGlobals();
  simulate_kqueue_failure = true;
  EXPECT_THROW({
    auto loop = MacEventLoop(10);
  }, std::system_error);
}

TEST(MacEventLoopTest, AddFdSucceeds) {
  resetTestGlobals();
  MacEventLoop loop(10);
  EXPECT_NO_THROW(loop.add_fd(100, [](int){ /* do nothing */ }));
}

TEST(MacEventLoopTest, AddFdThrowsOnKeventFailure) {
  resetTestGlobals();
  MacEventLoop loop(10);
  simulate_kevent_failure = true;
  EXPECT_THROW(loop.add_fd(100, [](int){ }), std::system_error);
  simulate_kevent_failure = false;
}

TEST(MacEventLoopTest, RemoveFdSucceeds) {
  resetTestGlobals();
  MacEventLoop loop(10);
  loop.add_fd(100, [](int){ });
  EXPECT_NO_THROW(loop.remove_fd(100));
}

TEST(MacEventLoopTest, RemoveFdThrowsOnKeventFailure) {
  resetTestGlobals();
  MacEventLoop loop(10);
  loop.add_fd(100, [](int){ });
  simulate_kevent_failure = true;
  EXPECT_THROW(loop.remove_fd(100), std::system_error);
  simulate_kevent_failure = false;
}

// Test that run() processes events and calls the registered callback.
// (Since run() is an infinite loop while running_ is true, we run it in a thread,
// register a callback for fd 100 (which our fake kevent always returns) and then call stop() in the callback.)
TEST(MacEventLoopTest, RunProcessesEvents) {
  resetTestGlobals();
  MacEventLoop loop(10);
  bool callback_called = false;
  loop.add_fd(100, [&](int fd) {
    callback_called = true;
    loop.stop();  // Stop the loop after processing one event.
  });
  std::thread t([&](){ loop.run(); });
  t.join();
  EXPECT_TRUE(callback_called);
}

// Test that the destructor calls close() on the poll fd.
TEST(MacEventLoopTest, DestructorClosesPollFd) {
  resetTestGlobals();
  {
    MacEventLoop loop(10);
  }
  EXPECT_TRUE(close_called);
}

TEST(MacEventLoopTest, AddFdCallsKeventWithCorrectParameters) {
  resetTestGlobals();
  ::testing::StrictMock<MockKevent> mock;
  g_mockKevent = &mock;
  MacEventLoop loop(10);
  // We expect that add_fd() will call kevent() with a non-null changelist.
  EXPECT_CALL(mock,
              kevent(simulated_poll_fd, ::testing::NotNull(), 1,
                     nullptr, 0, nullptr))
      .WillOnce(::testing::Return(0));
  EXPECT_NO_THROW(loop.add_fd(100, [](int){ }));
  g_mockKevent = nullptr;
}

} // namespace
