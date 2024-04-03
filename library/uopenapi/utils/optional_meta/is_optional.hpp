#pragma once

namespace uopenapi::utils {
template <typename T>
constexpr bool is_optional = false;

template <typename T>
concept optional = requires { requires is_optional<T>; };
}  // namespace uopenapi::utils
