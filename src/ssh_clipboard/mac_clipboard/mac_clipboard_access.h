#ifndef _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_MAC_CLIPBOARD_ACCESS_H
#define _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_MAC_CLIPBOARD_ACCESS_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "src/ssh_clipboard/mac_clipboard/native_clipboard_access.h"
#include "ssh_clipboard/clipboard/clipboard_interface.h"

namespace ssh_clipboard::clipboard {
class MacClipboardAccess final : public Clipboard {
 public:
  explicit MacClipboardAccess();
  ~MacClipboardAccess() override;
  MacClipboardAccess(const MacClipboardAccess&) = delete;
  MacClipboardAccess(MacClipboardAccess&&) = delete;
  MacClipboardAccess& operator=(const MacClipboardAccess&) = delete;
  MacClipboardAccess& operator=(MacClipboardAccess&&) = delete;

  void copy(const std::vector<uint8_t>& data) override;
  void on_copy(std::function<void(std::vector<uint8_t>&)> callback) override;
  auto paste() -> std::vector<uint8_t> override;

 private:
  void run();
  static constexpr std::chrono::duration kPollInterval =
      std::chrono::milliseconds(100);
  std::atomic<bool> running_;
  src_ssh_clipboard_mac_clipboard_native_clipboard_access::ClipboardMonitor
      monitor_;
  std::function<void(std::vector<uint8_t>&)> on_copy_callback_;
  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable cv_;
};
}  // namespace ssh_clipboard::clipboard

#endif  // _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_MAC_CLIPBOARD_ACCESS_H
