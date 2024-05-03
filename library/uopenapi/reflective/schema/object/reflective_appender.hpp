#pragma once
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <userver/utils/overloaded.hpp>

#include "userver/formats/yaml/value_builder.hpp"

namespace uopenapi::reflective {
template <typename T>
requires uopenapi::reflective::reflectivable<T>
struct schema_appender<T, none_requirements> {
    static void place_definition(
        schema_view schema, userver::formats::yaml::ValueBuilder& type_node) {
        if (type_node.IsObject()) {
            return;
        }
        type_node = userver::formats::common::Type::kObject;
        type_node["type"] = "object";
        type_node["additionalProperties"] = "false";
        auto required_visiter = [&]<typename F, typename Info>() {
            std::string name = Info::name.AsString();
            auto field_node = type_node["properties"][name];
            auto required_node = type_node["required"];
            if (!required_node.IsArray()) {
                required_node = userver::formats::yaml::Type::kArray;
            }
            required_node.PushBack(name);
            call_append<T, Info::name, F>(schema.from_node(field_node));
        };
        auto optional_visiter = [&]<utils::optional F, typename Info>() {
            std::string name = Info::name.AsString();
            auto field_node = type_node["properties"][name];
            using raw_type = utils::optional_getter_t<F>;
            call_append<T, Info::name, raw_type>(schema.from_node(field_node));
        };
        auto visiter =
            userver::utils::Overloaded{required_visiter, optional_visiter};
        uopenapi::pfr_extension::visit_struct<T>(visiter);
    }
    static void append(schema_view schema, none_requirements = {}) {
        if (!schema.is_root()) {
            place_ref_to_type<T>(schema.cur_place);
        }
        std::string name_type = schema_type_name<T>();
        auto type_node = schema.root["components"]["schemas"][name_type];
        place_definition(schema, type_node);
    }
};
}  // namespace uopenapi::reflective
