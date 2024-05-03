#pragma once
#include <uopenapi/enum_helpers/introspector.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <userver/formats/yaml/value_builder.hpp>

namespace uopenapi::reflective {
template <typename Enum>
requires enum_helpers::has_introspector<Enum>
struct schema_appender<Enum, none_requirements> {
    static void place_definition(
        userver::formats::yaml::ValueBuilder& type_node) {
        if (type_node.IsObject()) {
            return;
        }
        type_node = userver::formats::common::Type::kObject;
        type_node["type"] = "string";
        auto enum_node = type_node["enum"];
        enum_node = userver::formats::common::Type::kArray;
        for (auto name : enum_helpers::enum_introspector<Enum>::names) {
            enum_node.PushBack(name);
        }
    }

    static void append(schema_view schema, none_requirements) {
        if (!schema.is_root()) {
            place_ref_to_type<Enum>(schema.cur_place);
        }
        std::string name_type = schema_type_name<Enum>();
        auto type_node = schema.root["components"]["schemas"][name_type];
        place_definition(type_node);
    }
};
}  // namespace uopenapi::reflective
