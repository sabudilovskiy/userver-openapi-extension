#pragma once
#include <boost/uuid/uuid.hpp>
#include <uopenapi/reflective/schema/appender.hpp>

namespace uopenapi::reflective {

template <>
struct schema_appender<boost::uuids::uuid, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        userver::formats::yaml::ValueBuilder& cur = schema.cur_place;
        cur["type"] = "string";
        cur["format"] = "uuid";
    }
};
}  // namespace uopenapi::reflective
