#pragma once

#include <format>
#include <stdexcept>

namespace ps {
template <class... Args>
inline auto exception(std::format_string<Args...> fmt, Args&&... args) noexcept {
  return std::runtime_error(std::format(fmt, std::forward<Args>(args)...));
}
}  // namespace ps
