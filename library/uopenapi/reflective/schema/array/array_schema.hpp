#pragma once
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/reflective/requirements/array/array_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <vector>

namespace uopenapi::reflective{
    template <typename T>
    struct schema_appender<std::vector<T>, array_requirements>{
        template <array_requirements requirements>
        static void append(schema_view schema){
            auto& field_node = schema.cur_place;
            if (!field_node.IsObject()){
                field_node = userver::formats::common::Type::kObject;
            }
            field_node["type"] = "array";
            if (requirements.min_items){
                field_node["minItems"] = requirements.min_items.value();
            }
            if (requirements.max_items){
                field_node["maxItems"] = requirements.max_items.value();
            }
            if (requirements.unique_items)
            {
                field_node["uniqueItems"] = true;
            }
            auto items_node = field_node["items"];
            schema_appender<T, none_requirements>::append(schema.from_node(items_node));
        }
    };
    template <typename T>
    struct schema_appender<T, none_requirements>{
        template <array_requirements requirements>
        static void append(schema_view schema){
            auto& field_node = schema.cur_place;
            if (!field_node.IsObject()){
                field_node = userver::formats::common::Type::kObject;
            }
            auto items_node = field_node["items"];
            schema_appender<T, none_requirements>::append(schema.from_node(items_node));
        }
    };
}