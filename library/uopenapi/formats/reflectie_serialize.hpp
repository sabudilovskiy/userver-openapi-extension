#pragma once
#include <uopenapi/formats/utils.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <userver/formats/common/type.hpp>
#include <userver/formats/serialize/to.hpp>

namespace userver::formats::serialize {

template <typename T, typename Value>
requires uopenapi::reflective::reflectivable<T> &&
         uopenapi::formats::userver_formats_value<Value>
Value Serialize(const T& t, userver::formats::serialize::To<Value>) {
    typename Value::Builder builder = userver::formats::common::Type::kObject;
    auto one_field = [&]<typename Info, typename F>(const F& field) {
        auto name = Info::name.AsString();
        auto result =
            uopenapi::reflective::field_call_validate<T, Info::name>(field);
        if (!result) {
            throw uopenapi::utils::formatted_exception(
                "An error occurred while serializing the field. Field name: "
                "{}, message: {}",
                name, result.error_message());
        }
        if constexpr (uopenapi::utils::is_optional<F>) {
            if (!uopenapi::utils::optional_getter<F>::has_value(field)) {
                // Exiting lambda to prevent writing null
                return;
            }
        }
        builder[name] = field;
    };
    uopenapi::pfr_extension::for_each_named_field(t, one_field);
    return builder.ExtractValue();
}
}  // namespace userver::formats::serialize
