#pragma once
#include <boost/optional/optional.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::utils {
template <typename T>
constexpr bool is_optional<boost::optional<T>> = true;

    template <typename T>
    struct optional_getter<boost::optional<T>>{
        using value_type = T;
        static bool has_value(const boost::optional<T>& t){
            return t.has_value();
        }
        template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, boost::optional<T>>
        static decltype(auto) value(U&& u){
            return boost::forward<decltype(u)>(u).value();
        }

        static boost::optional<T> make_none(){
            return boost::none;
        }
    };
}