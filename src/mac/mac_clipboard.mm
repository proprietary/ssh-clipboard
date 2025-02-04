#import <Cocoa/Cocoa.h>
#include <string>
#include <string_view>
#include "mac_clipboard.h"

namespace ssh_clipboard {
  void Clipboard::copy(std::string_view text) {
    [pasteboard clearContents];
    NSString *nsText = [NSString stringWithUTF8String:text.data()];
    [pasteboard setString:nsText forType:NSPasteboardTypeString];
  }

  std::string Clipboard::paste() {
    NSString *nsText = [pasteboard stringForType:NSPasteboardTypeString];
    if (nsText == nil) {
      return "";
    }
    return [nsText UTF8String];
  }
} // namespace ssh_clipboard
