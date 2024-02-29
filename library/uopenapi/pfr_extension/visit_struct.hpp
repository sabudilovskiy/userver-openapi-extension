#pragma once
#include <boost/pfr.hpp>
#include <uopenapi/pfr_extension/field_info.hpp>

#define FWD(X) std::forward<decltype(X)>(X)

namespace uopenapi::pfr_extension{
    namespace details {
        template <typename T, std::size_t I>
        constexpr void visit_struct_one_field(auto& functor) {
            using Info = field_info<I, boost::pfr::get_name<I, T>()>;
            using Field = boost::pfr::tuple_element_t<I, T>;
            functor.template operator()<Field, Info>();
        }
        template <typename T, std::size_t... I>
        constexpr void visit_struct_helper(auto&& functor, std::integer_sequence<std::size_t, I...>) {
            (visit_struct_one_field<T, I>( functor), ...);
        }
    }
    template <typename U>
    constexpr void visit_struct(auto&& functor) {
        using T = std::remove_cvref_t<U>;
        constexpr std::size_t N = boost::pfr::tuple_size_v<T>;
        auto seq = std::make_index_sequence<N>{};
        details::visit_struct_helper<T>(FWD(functor), seq);
    }
}

#undef FWD