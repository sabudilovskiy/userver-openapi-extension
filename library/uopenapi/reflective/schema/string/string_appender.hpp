#pragma once
#include <uopenapi/reflective/requirements/string/string_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>

namespace uopenapi::reflective {
template <utils::ce::string format>
struct schema_appender<std::string, string_requirements<format>> {
    template <string_requirements<format> req>
    static void append(schema_view schema) {
        userver::formats::yaml::ValueBuilder& cur = schema.cur_place;
        cur["type"] = "string";
        if (!format.empty()) {
            cur["format"] = format.AsString();
        }
        if (req.min_length) {
            cur["minLength"] = *req.min_length;
        }
        if (req.max_length) {
            cur["maxLength"] = *req.max_length;
        }
        if (!req.pattern.empty()) {
            cur["pattern"] = req.pattern.AsString();
        }
    }
};

template <>
struct schema_appender<std::string, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        userver::formats::yaml::ValueBuilder& cur = schema.cur_place;
        cur["type"] = "string";
    }
};
}  // namespace uopenapi::reflective