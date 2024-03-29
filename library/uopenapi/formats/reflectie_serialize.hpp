#pragma once
#include <type_traits>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <userver/formats/serialize/to.hpp>

namespace userver::formats::serialize {

template <typename T, typename Value>
requires uopenapi::reflective::reflectivable<T>
Value Serialize(const T& t, userver::formats::serialize::To<Value>) {
    typename Value::Builder builder;
    auto one_field = [&]<typename Info>(auto& field) {
        auto name = Info::name.AsString();
        auto result =
            uopenapi::reflective::field_call_validate<T, Info::name>(field);
        if (!result) {
            throw uopenapi::utils::formatted_exception(
                "An error occurred while serializing the field. Field name: "
                "{}, message: {}",
                name, result.error_message());
        }
        builder[name] = field;
    };
    uopenapi::pfr_extension::for_each_named_field(t, one_field);
    return builder.ExtractValue();
}
}  // namespace userver::formats::serialize
