#pragma once

#pragma once

#include <uopenapi/reflective/requirements/number/number_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <numeric>

namespace uopenapi::reflective {
    template <>
    struct schema_appender<std::int32_t, number_requirements<std::int32_t>> {
        template <number_requirements<std::int32_t> req>
        static void append(schema_view schema) {
            auto& field_node = schema.cur_place;
            if (!field_node.IsObject()) {
                field_node = userver::formats::common::Type::kObject;
            }
            field_node["type"] = "integer";
            field_node["format"] = "int32";
            if (req.minimum) {
                field_node["minimum"] = *req.minimum;
            }
            if (req.exclusive_minimum) {
                field_node["exclusiveMinimum"] = true;
            }
            if (req.maximum) {
                field_node["maximum"] = *req.maximum;
            }
            if (req.exclusive_maximum) {
                field_node["exclusiveMaximum"] = true;
            }
            if (req.multiple_of){
                field_node["multipleOf"] = *req.multiple_of;
            }
        }
    };

    template <>
    struct schema_appender<std::int32_t, none_requirements> {
        template <none_requirements>
        static void append(schema_view schema) {
            auto& field_node = schema.cur_place;
            if (!field_node.IsObject()) {
                field_node = userver::formats::common::Type::kObject;
            }
            field_node["type"] = "integer";
            field_node["format"] = "int32";
        }
    };

}  // namespace uopenapi::reflective