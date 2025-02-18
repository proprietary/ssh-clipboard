#include "ssh_clipboard/event_loop/mac_event_loop.h"

namespace ssh_clipboard::event_loop {
auto create_event_loop(int max_events) -> std::unique_ptr<EventLoop> {
  return std::make_unique<MacEventLoop>(max_events);
}

MacEventLoop::MacEventLoop(int max_events)
    : poll_fd_{kqueue()},
      running_{false},
      events_{static_cast<size_t>(max_events)} {
  if (poll_fd_ < 0) {
    throw std::system_error(errno, std::generic_category(),
                            "Failed to create kqueue event loop");
  }
  callbacks_.reserve(max_events);
}

void MacEventLoop::run() {
  running_.store(true);
  while (running_) {
    const int n =
        kevent(poll_fd_, nullptr, 0, events_.data(), events_.size(), nullptr);
    if (n < 0) {
      throw std::system_error{errno, std::generic_category(),
                              "Failed to poll for events"};
    }
    for (int i = 0; i < n; ++i) {
      const auto fd = events_[i].ident;
      auto it = callbacks_.find(fd);
      if (it != callbacks_.end()) {
        it->second(fd);
      }
    }
  }
}

void MacEventLoop::stop() { running_.store(false); }

void MacEventLoop::add_fd(int fd, std::function<void(int)> callback) {
  struct kevent event{};
  memset(&event, 0, sizeof(event));
  EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);
  if (kevent(poll_fd_, &event, 1, nullptr, 0, nullptr) < 0) {
    throw std::system_error{errno, std::generic_category(),
                            "Failed to add fd to kqueue"};
  }
  callbacks_[fd] = std::move(callback);
}

void MacEventLoop::remove_fd(int fd) {
  struct kevent event{};
  memset(&event, 0, sizeof(event));
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  if (kevent(poll_fd_, &event, 1, nullptr, 0, nullptr) < 0) {
    throw std::system_error{errno, std::generic_category(),
                            "Failed to remove fd from kqueue"};
  }
  callbacks_.erase(fd);
}

MacEventLoop::~MacEventLoop() { close(poll_fd_); }
}  // namespace ssh_clipboard::event_loop
