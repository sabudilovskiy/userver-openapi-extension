#pragma once
#include <type_traits>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <userver/formats/json.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>

namespace userver::formats::serialize{

    template <typename T> requires uopenapi::reflective::reflectivable<T>
    formats::json::Value Serialize (const T& t, userver::formats::serialize::To<formats::json::Value>){
        formats::json::ValueBuilder json;
        auto one_field = [&]<typename Info>(auto& field) {
            auto name = Info::name.AsString();
            auto result = uopenapi::reflective::field_call_validate<T, Info::name>(field);
            if (!result){
                throw uopenapi::utils::formatted_exception(
                        "An error occurred while serializing the field. Field name: {}, message: {}",
                        name,
                        result.error_message());
            }
            json[name] = field;
        };
        uopenapi::pfr_extension::for_each_named_field(t, one_field);
        return json.ExtractValue();
    }
}