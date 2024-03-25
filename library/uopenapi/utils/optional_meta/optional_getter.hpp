#pragma once

namespace uopenapi::utils {
template <typename T>
struct optional_getter {
    //        static bool has_value(const T& t);
    //        template <typename U>
    //        requires std::is_same_v<std::remove_cvref_t<U>, T>
    //        static decltype(auto) value(U&& u)
    //        using value_type = X;
    //        static T make_none()
};

template <typename T>
using optional_getter_t = optional_getter<T>::value_type;
}  // namespace uopenapi::utils
