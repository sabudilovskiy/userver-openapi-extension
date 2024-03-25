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
    // can have static void append<requirements>(schema_view)
};

template <typename T, utils::ce::string name, typename appender>
concept field_exist_rt_append =
    requires(schema_view schema) {
        appender::append(schema, requirements_field<T, name>);
    };

template <typename T, utils::ce::string name, typename appender>
concept field_exist_nttp_append =
    requires(schema_view schema) {
        appender::template append<requirements_field<T, name>>(schema);
    };

template <typename T, typename appender>
concept exist_rt_append = requires(schema_view schema) {
                              appender::append(schema, requirements_type<T>);
                          };

template <typename T, utils::ce::string name, typename F>
void call_append(schema_view schema) {
    using appender = schema_appender<F, requirements_field_t<T, name>>;
    if constexpr (nttp_requirements_field<T, name>) {
        static_assert(field_exist_nttp_append<T, name, appender>,
                      "static void append<req>(schema) missing");
        return appender::template append<requirements_field<T, name>>(schema);
    } else if constexpr (field_exist_rt_append<T, name, appender>) {
        return appender::append(schema, requirements_field<T, name>);
    } else {
        static_assert(
            field_exist_rt_append<T, name, appender>,
            "Не найдена специализация appender с передачей requirements как "
            "нешаблонного аргумента."
            "Это возможно в трех случаях: "
            "1) У requirements отсутствует constexpr. Добавьте его. Если "
            "используете макрос, то правильный макрос: REQUIREMENTS_CE_UOPENAPI"
            "2) Специализация не видна в месте инстанцирования call_append() "
            "или не существует вообще"
            "3) Вы выставили type_requirements на тип, на который некорректно "
            "его выставлять.");
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
            static_assert(exist_rt_append<T, next_appender>,
                          "Не найдена специализация appender с передачей "
                          "requirements как нешаблонного аргумента."
                          "Это возможно в трех случаях: "
                          "1) У requirements отсутствует constexpr. Добавьте "
                          "его. Если используете макрос, то правильный макрос: "
                          "REQUIREMENTS_CE_UOPENAPI"
                          "2) Специализация не видна в месте инстанцирования "
                          "call_append() или не существует вообще"
                          "3) Вы выставили type_requirements на тип, на "
                          "который некорректно его выставлять.");
        }
    }
};

}  // namespace uopenapi::reflective
