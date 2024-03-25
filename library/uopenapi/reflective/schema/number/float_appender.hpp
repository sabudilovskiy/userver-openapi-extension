#pragma once

#include <numeric>
#include <uopenapi/reflective/requirements/number/number_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::reflective {
template <>
struct schema_appender<float, number_requirements<float>> {
    template <number_requirements<float> req>
    static void append(schema_view schema) {
        auto& field_node = schema.cur_place;
        if (!field_node.IsObject()) {
            field_node = userver::formats::common::Type::kObject;
        }
        field_node["type"] = "number";
        field_node["format"] = "float";
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
        if (req.multiple_of) {
            field_node["multipleOf"] = *req.multiple_of;
        }
    }
};

template <>
struct schema_appender<float, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        auto& field_node = schema.cur_place;
        if (!field_node.IsObject()) {
            field_node = userver::formats::common::Type::kObject;
        }
        field_node["type"] = "number";
        field_node["format"] = "float";
    }
};

}  // namespace uopenapi::reflective
