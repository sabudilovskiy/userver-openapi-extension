#pragma once
#include <uopenapi/pfr_extension/sorted_names.hpp>

namespace uopenapi::pfr_extension{
    //поле отсутствует
    struct place_holder{};

    namespace details{
        template <std::size_t I, typename T>
        decltype(auto) get_if_exist(T& t){
            if constexpr (boost::pfr::tuple_size_v<T> <= I){
                return place_holder{};
            }
            else {
                return boost::pfr::get<I>(t);
            }
        }


        template <utils::ce::string name>
        void visit_one_field(auto& functor, auto&... t){
            auto helper = []<typename U>(U& u) -> decltype(auto){
                return get_if_exist<get_index<U, name>()>(u);
            };
            functor.template operator()<name>(helper(t)...);
        }

        template <std::size_t... I, typename... T>
        void visit_all_helper(std::index_sequence<I...>, auto& functor, T&... t){
            constexpr auto set_of_names = get_set_of_names<std::remove_cvref_t<T>...>();
            auto visit_one_name = [&]<utils::ce::string name>(){
                visit_one_field<name>(functor, t...);
            };
            (visit_one_name.template operator()<set_of_names[I]>(), ...);
        }
    }

    template <typename... T>
    void visit_all(auto&& functor, T&&... t){
        constexpr auto N = get_size_set_of_names<std::remove_cvref_t<T>...>();
        details::visit_all_helper(std::make_index_sequence<N>{}, functor, t...);
    }
}