#pragma once
#include <type_traits>

namespace uopenapi::reflective {
template <typename T>
struct is_alias : std::false_type {};

template <typename T>
constexpr bool is_alias_v = is_alias<T>::value;

template <typename T>
struct alias_getter {
    // must have static auto&& get(auto&& t) -> native
};

}  // namespace uopenapi::reflective
