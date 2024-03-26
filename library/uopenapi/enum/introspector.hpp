#pragma once
#include <type_traits>

namespace uopenapi {
template <typename T>
concept HasIntrospector = requires {
    { get_enum_introspector(std::type_identity<T>{}) };
};

template <typename T>
using enum_introspector = std::remove_pointer_t<decltype(get_enum_introspector(
    std::type_identity<T>{}))>;
}  // namespace uopenapi
