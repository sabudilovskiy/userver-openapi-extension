#pragma once
#include <uopenapi/reflective/requirements/none_requirements.hpp>
#include <uopenapi/reflective/requirements/string/string_requirements.hpp>
#include <uopenapi/reflective/schema/appender.hpp>

namespace uopenapi::reflective {

template <>
struct schema_appender<std::string, none_requirements> {
    static void append(schema_view schema, none_requirements = {}) {
        auto& cur = schema.cur_place;
        if (!cur.IsObject()) {
            cur = userver::formats::common::Type::kObject;
        }
        cur["type"] = "string";
    }
};

template <utils::ce::string format>
struct schema_appender<std::string, string_requirements<format>> {
    static void append(schema_view schema,
                       const string_requirements<format>& req) {
        schema_appender<std::string, none_requirements>::append(schema);
        auto& cur = schema.cur_place;
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

}  // namespace uopenapi::reflective
