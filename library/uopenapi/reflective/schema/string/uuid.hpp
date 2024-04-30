#pragma once
#include <boost/uuid/uuid.hpp>
#include <uopenapi/reflective/schema/appender.hpp>

namespace uopenapi::reflective {

template <>
struct schema_appender<boost::uuids::uuid, none_requirements> {
    static void append(schema_view schema, none_requirements = {}) {
        auto& cur = schema.cur_place;
        if (!cur.IsObject()) {
            cur = userver::formats::common::Type::kObject;
        }
        cur["type"] = "string";
        cur["format"] = "uuid";
    }
};
}  // namespace uopenapi::reflective
