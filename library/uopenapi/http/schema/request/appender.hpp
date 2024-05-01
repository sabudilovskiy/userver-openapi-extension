#pragma once

#include <string_view>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
namespace uopenapi::http {
template <typename Field, typename Requirements, source_type SourceType>
struct schema_appender_request {
    // void append(reflective::schema_view, std::string_view, requirements);
};

template <typename Field, typename Requirements, source_type SourceType>
concept has_schema_appender_request =
    requires(reflective::schema_view schemaView, std::string_view name,
             const Requirements& requirements) {
        schema_appender_request<Field, Requirements, SourceType>::append(
            schemaView, name, requirements);
    };
}  // namespace uopenapi::http
