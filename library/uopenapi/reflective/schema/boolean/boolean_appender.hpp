#pragma once
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::reflective {
template <>
struct schema_appender<bool, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        if (!schema.cur_place.IsObject()) {
            schema.cur_place = userver::formats::yaml::Type::kObject;
        }
        schema.cur_place["type"] = "boolean";
    }
};
}  // namespace uopenapi::reflective
