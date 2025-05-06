#ifndef _INCLUDE_SSH_CLIPBOARD_WINDOWS_CLIPBOARD_IMPL_H
#define _INCLUDE_SSH_CLIPBOARD_WINDOWS_CLIPBOARD_IMPL_H
#include <windows.h>

#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

namespace ssh_clipboard {
class Clipboard {
 public:
  explicit Clipboard() {}

  void copy(std::string_view text);
  auto paste() -> std::string;
};
}  // namespace ssh_clipboard
#endif  // _INCLUDE_SSH_CLIPBOARD_WINDOWS_CLIPBOARD_IMPL_H
