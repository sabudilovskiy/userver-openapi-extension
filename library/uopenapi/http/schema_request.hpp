#pragma once

#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/http/schema/request/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/pfr_extension/get_index.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
namespace details {

template <typename T>
void append_request(reflective::schema_view schemaView) {
    auto& [root, cur] = schemaView;
    if (cur.IsObject()) {
        return;
    }
    cur["description"] = reflective::schema_type_name<T>();
    // clang-format off
    auto visitor = 
    [schemaView]
        <
            typename Field, typename Info,
            typename Requirements = reflective::requirements_field_t<T, Info::name>,
            auto st = http::field_source<T, Info::name>
        >
        requires has_schema_appender_request<Field, Requirements, st>
        () 
    {
        using appender = http::schema_appender_request<Field, Requirements, st>;
        const auto& req = reflective::requirements_field<T, Info::name>;
        appender::append(schemaView, Info::name.AsStringView(), req);
    };
    // clang-format on
    pfr_extension::visit_struct<T>(visitor);
}
}  // namespace details
}  // namespace uopenapi::http
