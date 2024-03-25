#pragma once
#include <concepts>
#include <optional>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::utils {
template <typename T>
constexpr bool is_optional<std::optional<T>> = true;

template <typename T>
struct optional_getter<std::optional<T>> {
    using value_type = T;
    static bool has_value(const std::optional<T>& t) { return t.has_value(); }
    template <typename U>
    requires std::is_same_v<std::remove_cvref_t<U>, std::optional<T>>
    static decltype(auto) value(U&& u) {
        return std::forward<decltype(u)>(u).value();
    }

    static std::optional<T> make_none() { return std::nullopt; }
};
}  // namespace uopenapi::utils