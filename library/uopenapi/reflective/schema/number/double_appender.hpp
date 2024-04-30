#pragma once

#include <numeric>
#include <uopenapi/reflective/requirements/none_requirements.hpp>
#include <uopenapi/reflective/requirements/number/number_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::reflective {

template <>
struct schema_appender<double, none_requirements> {
    static void append(schema_view schema, none_requirements = {}) {
        auto& field_node = schema.cur_place;
        if (!field_node.IsObject()) {
            field_node = userver::formats::common::Type::kObject;
        }
        field_node["type"] = "number";
        field_node["format"] = "double";
    }
};

template <>
struct schema_appender<double, number_requirements<double>> {
    static void append(schema_view schema,
                       const number_requirements<double>& req) {
        auto& field_node = schema.cur_place;
        schema_appender<double, none_requirements>::append(schema);
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

}  // namespace uopenapi::reflective
