#pragma once
#include <type_traits>
#include <uopenapi/utils/fallback_type.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>

namespace uopenapi::utils {
template <typename T>
struct optional_getter {
    using value_type = utils::fallback_type;
    static bool has_value(const T&) {
        static_assert(sizeof(T) == 0,
                      "Fallback has_value(). Write specialization");
        return true;
    }
    template <typename U>
    requires std::is_same_v<std::remove_cvref_t<U>, T>
    static decltype(auto) value(U&&) {
        static_assert(sizeof(T) == 0, "Fallback value(). Write specialization");
    }
    static T make_none() {
        static_assert(sizeof(T) == 0,
                      "Fallback make_none(). Write specialization");
        return T{};
    }
};

template <typename T>
using optional_getter_t = typename optional_getter<T>::value_type;

template <typename T, typename ValueType>
constexpr bool opt_underlying_of =
    std::is_same_v<optional_getter_t<T>, ValueType>;

template <typename T>
using opt_raw_type_t =
    std::conditional_t<is_optional<T>, optional_getter_t<T>, T>;

}  // namespace uopenapi::utils
