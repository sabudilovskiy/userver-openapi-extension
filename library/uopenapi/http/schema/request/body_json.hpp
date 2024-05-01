#pragma once
#include <uopenapi/http/schema/request/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
namespace uopenapi::http {

template <typename T, typename Requirements>
requires(
    reflective::has_schema_appender<utils::opt_raw_type_t<T>, Requirements>)
struct schema_appender_request<T, Requirements,
                               uopenapi::http::source_type::body_JSON> {
    using raw_type = utils::opt_raw_type_t<T>;
    using appender = reflective::schema_appender<raw_type, Requirements>;

    static void append(reflective::schema_view schemaView, std::string_view,
                       const Requirements& requirements) {
        auto& [root, cur] = schemaView;
        auto request_body_node = cur["requestBody"];
        if (!request_body_node.IsObject()) {
            request_body_node = userver::formats::yaml::Type::kObject;
        }
        request_body_node["required"] = !utils::is_optional<T>;
        auto content = request_body_node["content"];
        if (!content.IsObject()) {
            content = userver::formats::yaml::Type::kObject;
        }
        auto application_json_node = content["application/json"];
        if (application_json_node.IsObject()) {
            application_json_node = userver::formats::yaml::Type::kObject;
        }
        auto schema = application_json_node["schema"];
        appender::append(schemaView.from_node(schema), requirements);
    }
};
}  // namespace uopenapi::http
