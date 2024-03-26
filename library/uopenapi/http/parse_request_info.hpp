#pragma once
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/meta_request.hpp>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <userver/formats/json/value.hpp>

namespace uopenapi::http {
namespace details {
template <typename T>
T parse_json_body(std::string_view body) {
    return userver::formats::json::FromString(body).As<T>();
}

template <typename T, typename Field, utils::ce::string name>
Field parse_cookie(const request_info::cookies_map& cookies) {
    using parser = request_parser<Field, source_type::cookie>;

    auto c_name = name.AsString();
    auto c_it = cookies.find(c_name);
    if (c_it == cookies.end()) {
        return parser::missing(c_name);
    }
    auto res = parser::parse(c_it->second, name.AsStringView());
    reflective::is_validate_result auto validate_result =
        reflective::field_call_validate<T, name>(res);
    if (validate_result.has_error()) {
        throw utils::formatted_exception(
            "Failed validate cookie with name: [{}], error: [{}]", name,
            validate_result.error_message());
    }
    return std::move(res);
}

template <typename T, typename Field, utils::ce::string name>
Field parse_header(const request_info::headers_map& headers) {
    using parser = request_parser<Field, source_type::header>;

    auto h_name = name.AsString();
    auto h_it = headers.find(h_name);
    if (h_it == headers.end()) {
        return parser::missing(h_name);
    }
    auto res = parser::parse(h_it->second, h_name);
    reflective::is_validate_result auto validate_result =
        reflective::field_call_validate<T, name>(res);
    if (validate_result.has_error()) {
        throw utils::formatted_exception(
            "Failed validate header with name: [{}], error: [{}]", name,
            validate_result.error_message());
    }
    return std::move(res);
}

template <typename T, typename Field, utils::ce::string name>
Field parse_queries(const request_info::queries_map& queries) {
    using parser = request_parser<Field, source_type::query>;

    auto q_name = name.AsString();
    auto q_it = queries.find(q_name);
    if (q_it == queries.end()) {
        return parser::missing(q_name);
    }
    Field res = parser::parse(q_it->second, q_name);
    reflective::is_validate_result auto validate_result =
        reflective::field_call_validate<T, name>(res);
    if (validate_result.has_error()) {
        throw utils::formatted_exception(
            "Failed validate query with name: [{}], error: [{}]", name,
            validate_result.error_message());
    }
    return res;
}
}  // namespace details

template <typename T>
T parse_from_request(const request_info& req_info) {
    static_assert(count_bodies<T>() <= 1, "maximum one body in request");
    T t;
    auto visit_all = [&]<typename Info, typename F>(F& f) {
        constexpr source_type st = field_source<T, Info::name>;
        if constexpr (st == source_type::body) {
            f = details::parse_json_body<F>(req_info.body);
        } else if constexpr (st == source_type::cookie) {
            f = details::parse_cookie<T, F, Info::name>(req_info.cookies);
        } else if constexpr (st == source_type::header) {
            f = details::parse_header<T, F, Info::name>(req_info.headers);
        } else if constexpr (st == source_type::query) {
            f = details::parse_queries<T, F, Info::name>(req_info.queries);
        } else {
            static_assert(st == source_type::body, "unreachable");
        }
    };
    pfr_extension::for_each_named_field(t, visit_all);
    return t;
}
}  // namespace uopenapi::http
