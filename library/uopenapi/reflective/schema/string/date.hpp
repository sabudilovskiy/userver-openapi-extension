#pragma once
#include <uopenapi/reflective/schema/appender.hpp>
#include <userver/utils/datetime/date.hpp>

namespace uopenapi::reflective {
template <>
struct schema_appender<userver::utils::datetime::Date, none_requirements> {
    static void append(schema_view schemaView, none_requirements = {}) {
        auto& cur = schemaView.cur_place;
        if (!cur.IsObject()) {
            cur = userver::formats::common::Type::kObject;
        }
        cur["type"] = "string";
        cur["format"] = "date";
    }
};
}  // namespace uopenapi::reflective
