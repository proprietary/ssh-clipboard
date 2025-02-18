#ifndef _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_CLIPBOARD_INTERFACE_H
#define _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_CLIPBOARD_INTERFACE_H

// #include <concepts>
#include <vector>
#include <functional>
#include <memory>

namespace ssh_clipboard::clipboard {
  // template <typename PlatformImpl>
  // concept Clipboard = requires(PlatformImpl platform_impl, std::vector<uint8_t> data) {
  //   { platform_impl.on_copy([](std::vector<uint8_t>&) {}) } -> std::same_as<void>;
  //   { platform_impl.paste() } -> std::convertible_to<std::string>;
  //   { platform_impl.copy(data) } -> std::same_as<void>;
  // };

  class Clipboard {
  public:
    virtual ~Clipboard() = default;
    virtual void copy(const std::vector<uint8_t>& data) = 0;
    virtual auto paste() -> std::vector<uint8_t> = 0;
    virtual void on_copy(std::function<void(std::vector<uint8_t>&)> callback) = 0;
  };

  auto create_clipboard() -> std::unique_ptr<Clipboard>;

} // namespace ssh_clipboard::clipboard

#endif // _INCLUDE_SSH_CLIPBOARD_CLIPBOARD_CLIPBOARD_INTERFACE_H
