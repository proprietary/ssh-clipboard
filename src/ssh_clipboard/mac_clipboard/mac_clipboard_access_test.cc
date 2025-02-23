#include "src/ssh_clipboard/mac_clipboard/mac_clipboard_access.h"

#include <gtest/gtest.h>

#include <atomic>
#include <latch>
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

TEST(MacClipboardAccess,
     CopyIsNotifiedViaOnCopyCallbackIfContentOfSystemClipboardChanges) {
  ssh_clipboard::clipboard::MacClipboardAccess clipboard{};
  std::vector<uint8_t> data;
  const std::string test_string{"Lorem ipsum"};
  std::latch has_been_notified{1};
  clipboard.on_copy([&data, &has_been_notified](std::vector<uint8_t>& result) {
    data.swap(result);
    result.clear();
    has_been_notified.count_down();
  });
  clipboard.copy({test_string.begin(), test_string.end()});
  auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(1);
  while (!has_been_notified.try_wait() &&
         std::chrono::system_clock::now() < deadline) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
  }
  ASSERT_TRUE(has_been_notified.try_wait())
      << "Timed out waiting for on_copy callback to be called";
  EXPECT_EQ(test_string, std::string(data.begin(), data.end()));
}
}  // namespace
