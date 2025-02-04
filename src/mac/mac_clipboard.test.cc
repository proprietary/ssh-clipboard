#include "gtest/gtest.h"
#include <string>
#include "mac_clipboard.h"

TEST(ClipboardTest, CopyAndPaste) {
    std::string testContent = "hello world!";

    ssh_clipboard::Clipboard clipboard{};
    clipboard.copy(testContent);

    std::string clipboardContent = clipboard.paste();

    EXPECT_EQ(clipboardContent, testContent);
}
