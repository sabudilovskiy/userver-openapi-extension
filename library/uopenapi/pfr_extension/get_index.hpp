#pragma once
#include <boost/pfr.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::pfr_extension {
template <typename T, utils::ce::string name>
consteval std::size_t get_index() {
    constexpr auto names = boost::pfr::names_as_array<T>();
    for (std::size_t i = 0; i < names.size(); i++) {
        if (name.AsStringView() == names[i]) {
            return i;
        }
    }
    return std::size_t(-1);
}

template <utils::ce::string name, typename T>
constexpr decltype(auto) get_by_name(T&& t) {
    constexpr auto I = get_index<T, name>;
    static_assert(I < boost::pfr::tuple_size_v<T>,
                  "name don't provided in tuple");
    return boost::pfr::get<I>(std::forward<T>(t));
}

template <typename T, utils::ce::string name>
using tuple_element_name_t = boost::pfr::tuple_element_t<get_index<T, name>(), T>;
}  // namespace uopenapi::pfr_extension