#pragma once

#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
    namespace details {

        template<source_type SourceType>
        std::string get_in_string() {
            if constexpr (SourceType == source_type::header) {
                return "header";
            } else if constexpr (SourceType == source_type::cookie) {
                return "cookie";
            } else if constexpr (SourceType == source_type::query) {
                return "query";
            } else {
                static_assert(SourceType != source_type::body, "wtf");
            }
        }

        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> != source_type::body)
        void append_request_field(reflective::schema_view schemaView) {
            using F = pfr_extension::tuple_element_name_t<T, name>;
            auto &[root, cur] = schemaView;
            auto parameters_node = cur["parameters"];
            if (!parameters_node.IsArray()) {
                parameters_node = userver::formats::yaml::Type::kArray;
            }
            userver::formats::yaml::ValueBuilder parameter_node =
                    userver::formats::yaml::Type::kObject;
            parameter_node["in"] = get_in_string<uopenapi::http::field_source<T, name>>();
            parameter_node["name"] = name.AsString();
            parameter_node["required"] = userver::meta::kIsOptional<F>;
            auto schema = parameter_node["schema"];
            schema = userver::formats::yaml::Type::kObject;
            if constexpr (utils::is_optional<F>) {
                using raw_type = utils::optional_getter_t<F>;
                reflective::call_append<T, name, raw_type>(schemaView.from_node(schema));
            } else {
                reflective::call_append<T, name, F>(schemaView.from_node(schema));
            }
            parameters_node.PushBack(std::move(parameter_node));
        }

        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> == source_type::body)
        void append_request_field(reflective::schema_view schemaView) {
            using F = pfr_extension::tuple_element_name_t<T, name>;
            auto &[root, cur] = schemaView;
            auto request_body_node = cur["requestBody"];
            if (!request_body_node.IsObject()) {
                request_body_node = userver::formats::yaml::Type::kObject;
            }
            request_body_node["required"] = !utils::is_optional<F>;
            auto content = request_body_node["content"];
            if (!content.IsObject()) {
                content = userver::formats::yaml::Type::kObject;
            }
            auto application_json_node = content["application/json"];
            if (application_json_node.IsObject()) {
                application_json_node = userver::formats::yaml::Type::kObject;
            }
            auto schema = application_json_node["schema"];
            if constexpr (utils::is_optional<F>) {
                using raw_type = utils::optional_getter_t<T>;
                reflective::call_append<T, name, raw_type>(schemaView.from_node(schema));
            } else {
                reflective::call_append<T, name, F>(schemaView.from_node(schema));
            }
        }

        template <typename T>
        void append_request(reflective::schema_view schemaView)
        {
            auto& [root, cur] = schemaView;
            if (cur.IsObject())
            {
                return;
            }
            cur["description"] = reflective::schema_type_name<T>();
            auto visitor = [schemaView]<typename, typename Info>(){
                append_request_field<T, Info::name>(schemaView);
            };
            pfr_extension::visit_struct<T>(visitor);
        }
    }
}