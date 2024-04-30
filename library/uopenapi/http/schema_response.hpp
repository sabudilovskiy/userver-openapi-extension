#pragma once

#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/schema/response/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
namespace details {

template <typename T, std::size_t I>
void append_response(reflective::schema_view schemaView) {
    if (!schemaView.cur_place.IsObject()) {
        schemaView.cur_place = userver::formats::yaml::Type::kObject;
    }
    auto this_resp = schemaView.cur_place[std::to_string(I)];
    if (this_resp.IsObject()) {
        throw std::runtime_error("dublicated code in responses");
    }
    this_resp = userver::formats::yaml::Type::kObject;
    reflective::place_ref_to_response<T>(this_resp);
    userver::formats::yaml::ValueBuilder response =
        schemaView
            .root["components"]["responses"][reflective::schema_type_name<T>()];
    if (response.IsObject()) {
        return;
    }
    response["description"] = "";
    auto viewResponse = schemaView.from_node(response);
    auto visitor = [viewResponse]<typename Field, typename Info>() {
        /*
        This code is specifically written to be more gcc friendly. Explicit use
        of Info::name leads to a segfault, auto instead of type types in st and
        Info::name leads to absurd errors.
        */
        constexpr utils::ce::string name = Info::name;

        using Requirements = reflective::requirements_field_t<T, name>;
        constexpr source_type st = http::field_source<T, name>;
        const auto& req = reflective::requirements_field<T, name>;

        using appender =
            http::schema_appender_response<Field, Requirements, st>;

        auto checked_append = [&]<typename = void>
            requires has_schema_appender_response<Field, Requirements, st>
            () {
            appender::append(viewResponse, name.AsStringView(), req);
        };
        checked_append();
    };
    pfr_extension::visit_struct<T>(visitor);
}

}  // namespace details
}  // namespace uopenapi::http
