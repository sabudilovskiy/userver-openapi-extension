#pragma once
#include "boost/pfr.hpp"
#include "field_info.hpp"

#define FWD(X) std::forward<decltype(X)>(X)

namespace uopenapi::pfr_extension{
    namespace details {
        template <std::size_t I>
        constexpr void one_field_consume(auto& t, auto& functor) {
            using T = std::remove_cvref_t<decltype(t)>;
            auto info = field_info<I, boost::pfr::get_name<I, T>()>();
            functor(boost::pfr::get<I>(t), info);
        }
        template <std::size_t... I>
        constexpr void for_each_named_field_helper(
                auto&& t, auto&& functor, std::integer_sequence<std::size_t, I...>) {
            (one_field_consume<I>(t, functor), ...);
        }
    }
    constexpr void for_each_named_field(auto&& t, auto&& functor) {
        using T = std::remove_cvref_t<decltype(t)>;
        constexpr std::size_t N = boost::pfr::tuple_size_v<T>;
        auto seq = std::make_index_sequence<N>{};
        details::for_each_named_field_helper(FWD(t), FWD(functor), seq);
    }
}

#undef FWD