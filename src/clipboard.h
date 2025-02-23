#ifndef _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_H
#define _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_H

#include <concepts>
#include <string>
#include <string_view>

namespace ssh_clipboard {
template <typename ClipboardType>
concept Clipboard = requires(ClipboardType c, std::string_view sv) {
  { c.paste() } -> std::convertible_to<std::string>;
  { c.copy(sv) };
};
} // namespace ssh_clipboard
#endif // _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_H
