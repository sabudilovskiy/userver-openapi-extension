#pragma once

#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
namespace uopenapi::http {
template <typename T, typename Requirements, source_type SourceType>
struct schema_appender_response {
    // void append(schema_view, std::string_view, requirements);
};

template <typename T, typename Requirements, source_type SourceType>
concept has_schema_appender_response =
    requires(reflective::schema_view schemaView, std::string_view name,
             const Requirements& requirements) {
        schema_appender_response<T, Requirements, SourceType>::append(
            schemaView, name, requirements);
    };
}  // namespace uopenapi::http
