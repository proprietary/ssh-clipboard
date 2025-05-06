#include "windows_clipboard_impl.h"

namespace ssh_clipboard {
auto Clipboard::paste() -> std::string {}
void Clipboard::copy(std::string_view text) {}
}  // namespace ssh_clipboard
