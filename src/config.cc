#include "config.h"

namespace ssh_clipboard::config {
  Environment::Environment(char *envp[]) {
    for (char **it = envp; *it != nullptr; ++it) {
      std::string_view env{*it};
      auto pos = env.find('=');
      if (pos == std::string_view::npos) {
        continue;
      }
      auto key = env.substr(0, pos);
      auto value = env.substr(pos + 1);
      vars.emplace(key, value);
    }
  }

  std::string_view Environment::operator[](std::string_view key) const {
    const auto it = vars.find(key);
    if (it == vars.end()) {
      return {};
    }
    return it->second;
  }
} // namespace ssh_clipboard::config
