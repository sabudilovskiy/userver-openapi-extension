#pragma once
#include <uopenapi/utils/is_optional/is_optional.hpp>
#include <optional>

namespace uopenapi::utils{
    template <typename T>
    constexpr bool is_optional<std::optional<T>> = true;
}