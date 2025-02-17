#ifndef _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H
#define _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H

#include <functional>
#include <utility>
#include <concepts>

namespace ssh_clipboard::event_loop {
  template<typename FileDescriptorT>
  using Callback = std::function<void(FileDescriptorT)>;

  template<typename T, typename FileDescriptorT>
  concept EventLoop = requires(T t, int max_events) {
    { t.run() } -> std::same_as<void>;
    { t.stop() } -> std::same_as<void>;
    { T(max_events) } -> std::same_as<T>;
    { t.add_fd(std::declval<FileDescriptorT>(), std::declval<Callback<FileDescriptorT>>()) } -> std::same_as<void>;
    { t.remove_fd(std::declval<FileDescriptorT>()) } -> std::same_as<void>;
  };

  auto create_event_loop(int max_events);
} // namespace ssh_clipboard::event_loop

#endif // _INCLUDE_SSH_CLIPBOARD_EVENT_LOOP_EVENT_LOOP_H
