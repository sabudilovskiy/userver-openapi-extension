#pragma once

#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/http/schema/request/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/pfr_extension/get_index.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/constexpr_string.hpp>
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
    auto visitor = [schemaView]<typename Field, typename Info>() {
        /*
        This code is specifically written to be more gcc friendly. Explicit use
        of Info::name leads to a segfault, auto instead of type types in st and
        Info::name leads to absurd errors.
        */
        constexpr utils::ce::string name = Info::name;

        using Requirements = reflective::requirements_field_t<T, name>;
        constexpr source_type st = http::field_source<T, name>;
        const auto& req = reflective::requirements_field<T, name>;

        using appender = http::schema_appender_request<Field, Requirements, st>;

        auto checked_append = [&]<typename = void>
            requires has_schema_appender_request<Field, Requirements, st>
            () {
            appender::append(schemaView, name.AsStringView(), req);
        };
        checked_append();
    };
    pfr_extension::visit_struct<T>(visitor);
}
}  // namespace details
}  // namespace uopenapi::http
