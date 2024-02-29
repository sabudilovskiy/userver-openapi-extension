#pragma once

#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/reflective/schema/schema_type_name.hpp>
#include <uopenapi/reflective/requirements/requirements_type.hpp>

namespace uopenapi::reflective{
    template <typename T>
    void place_ref_to_type(userver::formats::yaml::ValueBuilder& place){
        std::string result = "components/schemas/";
        result += schema_type_name<T>();
        if (place.IsObject()){
            place = userver::formats::yaml::Type::kObject;
        }
        place["$ref"] = result;
    }

    template <typename T, typename requirements>
    struct schema_appender{
        //can have static void append(schema_view, requirements)
        //can have static void append<requirements>(schema_view)
    };

    template <typename T, utils::ce::string name, typename appender>
    concept field_exist_append = requires(schema_view schema){
        appender::append(schema, requirements_field<T,name>);
    };

    template <typename T, typename appender>
    concept exist_rt_append = requires(schema_view schema){
        appender::append(schema, requirements_type<T>);
    };

    template <typename T, utils::ce::string name>
    void call_append(schema_view schema){
        using appender = schema_appender<T, decltype(requirements_field<T, name>)>;
        if constexpr (nttp_requirements_field<T, name>){
            return appender::append<requirements_field<T,name>>(schema);
        }
        else if constexpr (field_exist_append<T, name, appender>){
            return appender::append(schema, requirements_field<T,name>);
        }
        else {
            static_assert (field_exist_append<T, name, appender>,
                           "Не найдена специализация appender с передачей requirements как нешаблонного аргумента."
                           "Это возможно в трех случаях: "
                           "1) У requirements отсутствует constexpr. Добавьте его. Если используете макрос, то правильный макрос: REQUIREMENTS_CE_UOPENAPI"
                           "2) Специализация не видна в месте инстанцирования call_append() или не существует вообще"
                           "3) Вы выставили type_requirements на тип, на который некорректно его выставлять.");
        }
    }
    template <typename T> requires (!trivial_requirements_type<T>)
    struct schema_appender<T,  none_requirements>{
        template <none_requirements>
        static void append(schema_view schema){
            using next_appender = schema_appender<T, requirements_type_t<T>>;
            if constexpr (nttp_requirements_type<T>){
                next_appender::append<requirements_type<T>>(schema);
            }
            else if constexpr (exist_rt_append<T, next_appender>){
                next_appender::append(schema, requirements_type<T>);
            }
            else {
                static_assert (exist_rt_append<T, next_appender>,
                               "Не найдена специализация appender с передачей requirements как нешаблонного аргумента."
                               "Это возможно в трех случаях: "
                               "1) У requirements отсутствует constexpr. Добавьте его. Если используете макрос, то правильный макрос: REQUIREMENTS_CE_UOPENAPI"
                               "2) Специализация не видна в месте инстанцирования call_append() или не существует вообще"
                               "3) Вы выставили type_requirements на тип, на который некорректно его выставлять.");
            }
        }
    };

    template <typename T> requires uopenapi::reflective::reflectivable<T>
    struct schema_appender<T,  none_requirements>{
        void append(schema_view schema){
            if (!schema.is_root()){
                place_ref_to_type<T>(schema.cur_place);
            }
            std::string name_type = schema_type_name<T>();
            auto type_node = schema.root["components"]["schemas"][name_type];
            if (type_node.IsObject()){
                return;
            }
            type_node = userver::formats::common::Type::kObject;
            type_node["type"] = "object";
            auto visiter = [&]<typename F, typename Info>(Info){
                std::string name = Info::name.AsString();
                auto field_node = type_node["fields"][name];
                if (!utils::is_optional<F>){
                    auto required_node =  type_node["required"];
                    if (!required_node.IsArray()){
                        required_node = userver::formats::yaml::Type::kArray;
                    }
                    required_node.PushBack(name);
                }
                call_append<T, name>(schema.from_node(type_node));
            };
            uopenapi::pfr_extension::visit_struct<T>(visiter);
        }
    };
}