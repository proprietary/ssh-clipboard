#ifndef _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H
#define _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H

#include <concepts>
#include <functional>
#include <memory>
#include <utility>

namespace ssh_clipboard::event_loop {
class EventLoop {
 public:
  virtual ~EventLoop() = default;
  virtual void run() = 0;
  virtual void stop() = 0;
  virtual void add_fd(int fd, std::function<void(int)> callback) = 0;
  virtual void remove_fd(int fd) = 0;
};

auto create_event_loop(int max_events) -> std::unique_ptr<EventLoop>;
}  // namespace ssh_clipboard::event_loop

#endif  // _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H
