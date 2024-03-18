#pragma once
#include <uopenapi/reflective/schema/appender.hpp>

namespace uopenapi::reflective{
    template <typename T>
    requires uopenapi::reflective::reflectivable<T>
    struct schema_appender<T, none_requirements> {
        template <none_requirements>
        static void append(schema_view schema) {
            if (!schema.is_root()) {
                place_ref_to_type<T>(schema.cur_place);
            }
            std::string name_type = schema_type_name<T>();
            auto type_node = schema.root["components"]["schemas"][name_type];
            if (type_node.IsObject()) {
                return;
            }
            type_node = userver::formats::common::Type::kObject;
            type_node["type"] = "object";
            auto visiter = [&]<typename F, typename Info>() {
                std::string name = Info::name.AsString();
                auto field_node = type_node["properties"][name];
                if (!utils::is_optional<F>) {
                    auto required_node = type_node["required"];
                    if (!required_node.IsArray()) {
                        required_node = userver::formats::yaml::Type::kArray;
                    }
                    required_node.PushBack(name);
                }
                call_append<T, Info::name, F>(schema.from_node(field_node));
            };
            uopenapi::pfr_extension::visit_struct<T>(visiter);
        }
    };
}