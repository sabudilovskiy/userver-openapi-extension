#pragma once
#include <uopenapi/enum/introspector.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::reflective {
template <typename Enum>
requires HasIntrospector<Enum>
struct schema_appender<Enum, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        if (!schema.is_root()) {
            place_ref_to_type<Enum>(schema.cur_place);
        }
        std::string name_type = schema_type_name<Enum>();
        auto type_node = schema.root["components"]["schemas"][name_type];
        if (type_node.IsObject()) {
            return;
        }
        type_node = userver::formats::common::Type::kObject;
        type_node["type"] = "string";
        auto enum_node = type_node["enum"];
        enum_node = userver::formats::common::Type::kArray;
        for (auto name : enum_introspector<Enum>::names) {
            enum_node.PushBack(name);
        }
    }
};
}  // namespace uopenapi::reflective
