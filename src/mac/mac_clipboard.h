#import <Cocoa/Cocoa.h>
#include <string>
#include <string_view>

namespace ssh_clipboard {
  class Clipboard {
  public:
    explicit Clipboard() {
      pasteboard = [NSPasteboard generalPasteboard];
    }
    void copy(std::string_view text);
    std::string paste();
  private:
    NSPasteboard *pasteboard;
  };
} // namespace ssh_clipboard
