#pragma once
#include <type_traits>

namespace uopenapi {
template <typename T>
concept has_introspector = requires {
    { get_enum_introspector(std::type_identity<T>{}) };
};

template <typename T>
requires has_introspector<T>
using enum_introspector =
    decltype(get_enum_introspector(std::type_identity<T>{}));
}  // namespace uopenapi
