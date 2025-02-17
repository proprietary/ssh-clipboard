#ifndef _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_MAC_EVENT_LOOP_H
#define _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_MAC_EVENT_LOOP_H

#include <sys/event.h>
#include <sys/time.h>

#include <atomic>
#include <functional>
#include <system_error>

#include "absl/container/flat_hash_map.h"
#include "ssh_clipboard/event_loop/event_loop.h"

namespace ssh_clipboard::event_loop {
class MacEventLoop final : public EventLoop {
 public:
  MacEventLoop(int max_events);
  ~MacEventLoop() override;
  void run() override;
  void stop() override;
  void add_fd(int fd, std::function<void(int)> callback) override;
  void remove_fd(int fd) override;

 private:
  int poll_fd_;
  std::atomic<bool> running_;
  std::vector<struct kevent> events_;
  absl::flat_hash_map<int, std::function<void(int)>> callbacks_;
};
}  // namespace ssh_clipboard::event_loop

#endif  // _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_MAC_EVENT_LOOP_H
