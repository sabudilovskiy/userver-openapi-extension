#pragma once

#include <uopenapi/http/schema/response/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
namespace uopenapi::http {
template <typename T, typename Requirements>
struct schema_appender_response<T, Requirements, source_type::query> {
    void append(reflective::schema_view, std::string_view,
                const Requirements&) {
        static_assert(sizeof(Requirements) == 0,
                      "Queries cannot be in responses.");
    }
};
}  // namespace uopenapi::http
