#include "ssh_clipboard/event_loop/linux_event_loop.h"

namespace ssh_clipboard::event_loop {
auto create_event_loop(int max_events) -> std::unique_ptr<EventLoop> {
  return std::make_unique<LinuxEventLoop>(max_events);
}

LinuxEventLoop::LinuxEventLoop(int max_events)
    : epoll_fd_{-1}, running_{false}, events_{static_cast<size_t>(max_events)} {
  epoll_fd_ = epoll_create1(0);
  if (epoll_fd < 0) {
    throw std::system_error{errno, std::generic_category(),
                            "epoll_create1: Failed to create event loop"};
  }
  callbacks_.reserve(max_events);
}

LinuxEventLoop::~LinuxEventLoop() { close(epoll_fd_); }

void run() {
  running_.store(true);
  while (running_) {
    const int n = epoll_wait(epoll_fd_, events_.data(), events_.size(), -1);
    if (n < 0) {
      throw std::system_error{errno, std::generic_category(),
                              "epoll_wait: Failed to wait for events"};
    }
    for (int i = 0; i < n; ++i) {
      const auto fd = events_[i].data.fd;
      auto handler = callbacks_.find(fd);
      if (handler != callbacks_.end()) {
        handler->second(fd);
      }
    }
  }
}

void stop() { running_.store(false); }

void add_fd(int fd, std::function<void(int)> callback) {
  struct epoll_event event;
  memset(&event, 0, sizeof(event));
  event.events = EPOLLIN;
  event.data.fd = fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event) < 0) {
    throw std::system_error{errno, std::generic_category(),
                            "epoll_ctl: Failed to add file descriptor"};
  }
  callbacks_[fd] = std::move(callback);
}

void remove_fd(int fd, Callback callback) {
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
    throw std::system_error{errno, std::generic_category(),
                            "epoll_ctl: Failed to remove file descriptor"};
  }
  callbacks_.erase(fd);
}
}  // namespace ssh_clipboard::event_loop
