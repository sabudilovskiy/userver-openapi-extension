#pragma once
#include <optional>
#include <uopenapi/utils/is_optional/is_optional.hpp>

namespace uopenapi::utils {
template <typename T>
constexpr bool is_optional<std::optional<T>> = true;
}