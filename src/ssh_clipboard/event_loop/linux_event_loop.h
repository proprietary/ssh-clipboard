#ifndef _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H
#define _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H

#include "absl/container/flat_hash_map.h"
#include <sys/epoll.h>
#include <system_error>
#include <vector>
#include <atomic>

namespace ssh_clipboard::event_loop {

class LinuxEventLoop {
public:
  LinuxEventLoop(int max_events);
  ~LinuxEventLoop();
  void run();
  void stop();
  void add_fd(int fd, std::function<void(int)> callback);
  void remove_fd(int fd);
private:
  int epoll_fd_;
  std::atomic<bool> running_;
  std::vector<struct epoll_event> events_;
  absl::flat_hash_map<int, std::function<void(int)>> callbacks_;
};

  auto create_event_loop(int max_events) -> LinuxEventLoop {
    return LinuxEventLoop{max_events};
  }

} // namespace ssh_clipboard::event_loop

#endif // _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_LINUX_EVENT_LOOP_H
