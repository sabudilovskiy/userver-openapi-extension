#pragma once

#include <numeric>
#include <uopenapi/reflective/requirements/number/number_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::reflective {

template <typename T>
requires std::is_integral_v<T>
struct schema_appender<T, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        auto& field_node = schema.cur_place;
        if (!field_node.IsObject()) {
            field_node = userver::formats::common::Type::kObject;
        }
        field_node["type"] = "integer";
        field_node["minimum"] = std::numeric_limits<T>::min();
        field_node["maximum"] = std::numeric_limits<T>::max();
    }
};

template <typename T>
struct schema_appender<T, number_requirements<T>> {
    template <number_requirements<T> req>
    static void append(schema_view schema) {
        auto& field_node = schema.cur_place;
        if (!field_node.IsObject()) {
            field_node = userver::formats::common::Type::kObject;
        }
        field_node["type"] = "integer";
        if (req.minimum) {
            field_node["minimum"] = *req.minimum;
        } else {
            field_node["minimum"] = std::numeric_limits<T>::min();
        }
        if (req.maximum) {
            field_node["maximum"] = *req.maximum;
        } else {
            field_node["maximum"] = std::numeric_limits<T>::max();
        }
        if (req.exclusive_minimum) {
            field_node["exclusiveMinimum"] = req.exclusive_minimum;
        }
        if (req.exclusive_maximum) {
            field_node["exclusiveMaximum"] = req.exclusive_maximum;
        }
        if (req.multiple_of) {
            field_node["multipleOf"] = *req.multiple_of;
        }
    }
};

}  // namespace uopenapi::reflective