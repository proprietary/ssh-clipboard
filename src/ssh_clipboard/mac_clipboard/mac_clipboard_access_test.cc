#include "src/ssh_clipboard/mac_clipboard/mac_clipboard_access.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {
TEST(MacClipboardAccess, RoundTrip) {
  const std::string text = "Hello, World!";
  const std::vector<uint8_t> text_bytes(text.begin(), text.end());
  ssh_clipboard::clipboard::MacClipboardAccess clipboard{};
  clipboard.copy(text_bytes);
  const auto result = clipboard.paste();
  ASSERT_EQ(text_bytes, result);
}
}  // namespace
