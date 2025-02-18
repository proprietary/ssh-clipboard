#include "ssh_clipboard/mac_clipboard/mac_clipboard_access.h"

#include <string>

namespace {
auto from_swift_string(const swift::String& string) -> std::vector<uint8_t> {
  auto utf8_view = string.getUtf8();
  std::vector<uint8_t> result;
  result.reserve(utf8_view.getCount());
  for (auto idx = utf8_view.getStartIndex();
       idx.getEncodedOffset() < utf8_view.getEndIndex().getEncodedOffset();) {
    result.push_back(utf8_view[idx]);
    const auto new_index = utf8_view.indexAfter(idx);
    idx = new_index;
  }
  return result;
}
}  // namespace

namespace ssh_clipboard::clipboard {
using namespace src_ssh_clipboard_mac_clipboard_native_clipboard_access;

MacClipboardAccess::MacClipboardAccess()
    : running_{false},
      monitor_{ClipboardMonitor::init()},
      on_copy_callback_{[](std::vector<uint8_t>) {}},
      thread_{&MacClipboardAccess::run, this} {}

void MacClipboardAccess::run() {
  while (running_) {
    {
      std::unique_lock<std::mutex> lock{mutex_};
      if (monitor_.checkForChanges()) {
        swift::String text = monitor_.paste();
        auto data = from_swift_string(text);
        on_copy_callback_(data);
      }
    }
    if (running_) {
      std::this_thread::sleep_for(kPollInterval);
    }
  }
}

MacClipboardAccess::~MacClipboardAccess() {
  if (running_) {
    running_ = false;
  }
  if (thread_.joinable()) {
    thread_.join();
  }
}

void MacClipboardAccess::on_copy(
    std::function<void(std::vector<uint8_t>&)> callback) {
  on_copy_callback_ = std::move(callback);
}

void MacClipboardAccess::copy(const std::vector<uint8_t>& data) {
  std::string text(data.begin(), data.end());
  monitor_.copy(text);
}

auto MacClipboardAccess::paste() -> std::vector<uint8_t> {
  auto text = monitor_.paste();
  return from_swift_string(text);
}
}  // namespace ssh_clipboard::clipboard
