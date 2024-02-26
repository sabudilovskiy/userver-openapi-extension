#pragma once
#include <type_traits>
#include "uopenapi/reflective/requirements/requirements.hpp"
#include <uopenapi/reflective/reflectivable.hpp>
#include <userver/formats/json.hpp>

namespace userver::formats::serialize{

    template <typename T> requires uopenapi::reflective::reflectivable<T>
    formats::json::Value Serialize (const T& t, userver::formats::serialize::To<formats::json::Value>){
        formats::json::ValueBuilder json;
        auto one_field = [&]<typename Info>(auto& field, Info) {
            auto name = Info::name.AsString();
            uopenapi::reflective::call_validate<T, Info::name>(field);
            json[name] = field;
        };
        uopenapi::pfr_extension::for_each_named_field(t, one_field);
        return json.ExtractValue();
    }
}