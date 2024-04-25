#pragma once
#include <uopenapi/utils/fallback_type.hpp>

namespace uopenapi::utils {
template <typename T>
struct optional_getter {
    using value_type = utils::fallback_type;
    static bool has_value(const T&) {
        static_assert(sizeof(T) == 0,
                      "Fallback has_value(). Write specialization");
    }
    template <typename U>
    requires std::is_same_v<std::remove_cvref_t<U>, T>
    static decltype(auto) value(U&&) {
        static_assert(sizeof(T) == 0, "Fallback value(). Write specialization");
    }
    static T make_none() {
        static_assert(sizeof(T) == 0,
                      "Fallback make_none(). Write specialization");
    }
};

template <typename T>
using optional_getter_t = typename optional_getter<T>::value_type;
}  // namespace uopenapi::utils
