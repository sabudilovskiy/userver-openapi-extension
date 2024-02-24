#pragma once
#include <type_traits>
#include <uopenapi/reflective/requirements.hpp>
#include "userver/formats/json.hpp"

namespace userver::formats::parse{

    template <typename T> requires std::is_aggregate_v<T>
    T Parse (const userver::formats::json::Value& value, userver::formats::parse::To<T>){
        T t;
        auto one_field = [&]<typename Info>(auto& field, Info) {
            auto req = uopenapi::reflective::requirements<T, Info::name>;

            using F = std::remove_cvref_t<decltype(field)>;
            auto name = Info::name.AsStringView();
            field = value[name].template As<F>();
        };
        uopenapi::pfr_extension::for_each_named_field(t, one_field);
        return t;
    }
}