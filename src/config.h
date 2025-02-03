#ifndef _INCLUDE_SSH_CLIPBOARD_CONFIG_H
#define _INCLUDE_SSH_CLIPBOARD_CONFIG_H
#include <string_view>
#include <unordered_map>
#include <concepts>

namespace ssh_clipboard::config {

class Environment {
 private:
  std::unordered_map<std::string_view, std::string_view> vars;
 public:
  explicit Environment(char *envp[]);
  [[nodiscard]] std::string_view operator[](std::string_view key) const;
};

class YamlConfigFile {
public:
 std::string_view operator[](std::string_view key) const;
};

template <typename T>
concept ConfigProvider = requires(T t, std::string_view key) {
  { t[key] } -> std::same_as<std::string_view>;
  std::same_as<T, Environment> || std::same_as<T, YamlConfigFile>;
};

template <ConfigProvider C>
class Config {
private:
  C config_provider;
public:
  explicit Config(C config_provider) : config_provider{config_provider} {
  }

  void reload() {}
};
} // namespace ssh_clipboard::config
#endif // _INCLUDE_SSH_CLIPBOARD_CONFIG_H
