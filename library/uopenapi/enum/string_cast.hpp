#pragma once
#include <optional>
#include <string_view>
#include <uopenapi/enum/introspector.hpp>

namespace uopenapi {
template <typename T>
requires has_introspector<T>
constexpr std::optional<std::string_view> enum_to_string_view(const T& t) {
    std::size_t index = 0;
    for (auto val : enum_introspector<T>::values) {
        if (t == val) {
            break;
        }
        index++;
    }
    if (index == enum_introspector<T>::names.size()) {
        return std::nullopt;
    }
    return enum_introspector<T>::names[index];
}

template <typename T>
requires has_introspector<T>
constexpr std::optional<T> enum_from_string_view(std::string_view sv) {
    constexpr auto names = enum_introspector<T>::names;
    constexpr auto values = enum_introspector<T>::values;
    std::size_t index = 0;
    for (auto name : names) {
        if (sv == name) {
            break;
        }
        index++;
    }
    if (index == names.size())
        return std::nullopt;
    else
        return values[index];
}
}  // namespace uopenapi
