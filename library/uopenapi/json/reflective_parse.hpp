#pragma once
#include <type_traits>
#include "uopenapi/reflective/requirements/requirements.hpp"
#include <uopenapi/reflective/reflectivable.hpp>
#include "userver/formats/json.hpp"

namespace userver::formats::parse{

    template <typename T> requires uopenapi::reflective::reflectivable<T>
    T Parse (const userver::formats::json::Value& value, userver::formats::parse::To<T>){
        T t;
        auto one_field = [&]<typename Info>(auto& field, Info) {
            auto req = uopenapi::reflective::requirements<T, Info::name>;
            using F = std::remove_cvref_t<decltype(field)>;
            auto name = Info::name.AsStringView();
            field = value[name].template As<F>();
            validate(field, req);
        };
        uopenapi::pfr_extension::for_each_named_field(t, one_field);
        return t;
    }
}