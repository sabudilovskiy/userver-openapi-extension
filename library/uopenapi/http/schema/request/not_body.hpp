#pragma once
#include <uopenapi/http/schema/request/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename T, typename Requirements, source_type SourceType>
requires(
    details::not_body_type(SourceType) &&
    reflective::has_schema_appender<utils::opt_raw_type_t<T>, Requirements>)
struct schema_appender_request<T, Requirements, SourceType> {
    using raw_type = utils::opt_raw_type_t<T>;

    static void append(reflective::schema_view schemaView,
                       std::string_view name,
                       const Requirements& requirements) {
        auto& [root, cur] = schemaView;

        auto parameters_node = cur["parameters"];
        if (!parameters_node.IsArray()) {
            parameters_node = userver::formats::yaml::Type::kArray;
        }
        userver::formats::yaml::ValueBuilder parameter_node =
            userver::formats::yaml::Type::kObject;
        parameter_node["in"] = to_string_view(SourceType);
        parameter_node["name"] = name;
        parameter_node["required"] = userver::meta::kIsOptional<T>;
        auto schema = parameter_node["schema"];
        schema = userver::formats::yaml::Type::kObject;
        using appender = reflective::schema_appender<raw_type, Requirements>;
        appender::append(schemaView.from_node(schema), requirements);
        parameters_node.PushBack(std::move(parameter_node));
    }
};
}  // namespace uopenapi::http
