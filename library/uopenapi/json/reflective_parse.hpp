#pragma once
#include <type_traits>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <userver/formats/json.hpp>

namespace userver::formats::parse {

template <typename T>
requires uopenapi::reflective::reflectivable<T>
T Parse(const userver::formats::json::Value& value,
        userver::formats::parse::To<T>) {
    T t;
    auto one_field = [&]<typename Info>(auto& field) {
        using F = std::remove_cvref_t<decltype(field)>;
        auto name = Info::name.AsStringView();
        field = value[name].template As<F>();
        auto result =
            uopenapi::reflective::field_call_validate<T, Info::name>(field);
        if (!result) {
            throw uopenapi::utils::formatted_exception(
                "An error occurred while parsing the field on path: [{}]. "
                "Field name: [{}], message: [{}]",
                value.GetPath(), name, result.error_message());
        }
    };
    uopenapi::pfr_extension::for_each_named_field(t, one_field);
    return t;
}
}  // namespace userver::formats::parse
