#ifndef _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H
#define _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H

#include <sys/epoll.h>

#include <atomic>
#include <system_error>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "ssh_clipboard/event_loop/event_loop.h"

namespace ssh_clipboard::event_loop {

class LinuxEventLoop final : public EventLoop {
 public:
  LinuxEventLoop(int max_events);
  ~LinuxEventLoop() override;
  void run() override;
  void stop() override;
  void add_fd(int fd, std::function<void(int)> callback) overide;
  void remove_fd(int fd) override;

 private:
  int epoll_fd_;
  std::atomic<bool> running_;
  std::vector<struct epoll_event> events_;
  absl::flat_hash_map<int, std::function<void(int)>> callbacks_;
};

}  // namespace ssh_clipboard::event_loop

#endif  // _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H
