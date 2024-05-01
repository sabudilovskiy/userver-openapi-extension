#pragma once

#include <uopenapi/reflective/requirements/requirements_type.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/reflective/schema/schema_type_name.hpp>

namespace uopenapi::reflective {
template <typename T>
void place_ref_to_type(userver::formats::yaml::ValueBuilder& place) {
    std::string result = "#/components/schemas/";
    result += schema_type_name<T>();
    if (!place.IsObject()) {
        place = userver::formats::yaml::Type::kObject;
    }
    place["$ref"] = result;
}

template <typename T>
void place_ref_to_response(userver::formats::yaml::ValueBuilder& place) {
    std::string result = "#/components/responses/";
    result += schema_type_name<T>();
    if (!place.IsObject()) {
        place = userver::formats::yaml::Type::kObject;
    }
    place["$ref"] = result;
}

template <typename T, typename requirements>
struct schema_appender {
    // can have static void append(schema_view, requirements)
};

template <typename T, typename Requirements>
concept has_schema_appender = requires(schema_view sv, Requirements req) {
    schema_appender<T, Requirements>::append(sv, req);
};

template <typename T, utils::ce::string name, typename appender>
concept field_exist_rt_append = requires(schema_view schema) {
    appender::append(schema, requirements_field<T, name>);
};

template <typename T, typename appender>
concept exist_rt_append = requires(schema_view schema) {
    appender::append(schema, requirements_type<T>);
};

template <typename T, utils::ce::string name, typename F>
void call_append(schema_view schema) {
    using appender = schema_appender<F, requirements_field_t<T, name>>;
    if constexpr (field_exist_rt_append<T, name, appender>) {
        return appender::append(schema, requirements_field<T, name>);
    } else {
        static_assert(
            field_exist_rt_append<T, name, appender>,
            "The appender specialization that passed requirements as a "
            "non-template argument was not found."
            "This is possible in three cases: "
            "1) Requirements does not have constexpr. Add it. If "
            "you use a macro, then the macro is correct: "
            "REQUIREMENTS_CE_UOPENAPI"
            "2) Specialization is not visible at the instantiation location "
            "call_append() "
            "or doesn't exist at all"
            "3) You set type_requirements to type, which is incorrect "
            "exhibit it.");
    }
}
template <typename T>
requires(!trivial_requirements_type<T>)
struct schema_appender<T, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        using next_appender = schema_appender<T, requirements_type_t<T>>;
        if constexpr (nttp_requirements_type<T>) {
            next_appender::template append<requirements_type<T>>(schema);
        } else if constexpr (exist_rt_append<T, next_appender>) {
            next_appender::append(schema, requirements_type<T>);
        } else {
            static_assert(
                exist_rt_append<T, next_appender>,
                "The appender specialization that passed requirements as a "
                "non-template argument was not found."
                "This is possible in three cases: "
                "1) Requirements does not have constexpr. Add it. If "
                "you use a macro, then the macro is correct: "
                "REQUIREMENTS_CE_UOPENAPI"
                "2) Specialization is not visible at the instantiation "
                "location call_append() "
                "or doesn't exist at all"
                "3) You set type_requirements to type, which is incorrect "
                "exhibit it.");
        }
    }
};

}  // namespace uopenapi::reflective
