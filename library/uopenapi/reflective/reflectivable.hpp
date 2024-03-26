#pragma once

#include <type_traits>

namespace uopenapi::reflective {
// можно явно специализировать, чтобы изменить поведение
template <typename T>
constexpr bool reflectivable =
    std::is_aggregate_v<T> && UOPENAPI_DEFAULT_REFLECTIVABLE;
}  // namespace uopenapi::reflective
