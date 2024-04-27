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

template <typename T>
T parse_from_request(const request_info& req_info) {
    static_assert(count_bodies<T>() <= 1, "maximum one body in request");
    T t;
    auto visit_all = [&]<typename Info, typename Field>(Field& f) requires(
                         has_request_parser<Field, field_source<T, Info::name>>)
    {
        constexpr source_type st = field_source<T, Info::name>;
        using parser = request_parser<Field, st>;
        f = parser::parse(req_info, Info::name.AsStringView());
        reflective::is_validate_result auto validateResult = [&]() {
            if constexpr (utils::is_optional<T>) {
                if (!utils::optional_getter<T>::has_value(f)) {
                    return reflective::validate_result::ok();
                }
                return reflective::field_call_validate<T, Info::name>(
                    utils::optional_getter<T>::value(f));
            } else {
                return reflective::field_call_validate<T, Info::name>(f);
            }
        }();
        if (validateResult.has_error()) {
            throw utils::formatted_exception(
                "Failed validate {} field. Error: [{}].", to_string_view(st),
                validateResult.error_message());
        }
    };
    pfr_extension::for_each_named_field(t, visit_all);
    return t;
}
}  // namespace uopenapi::http
