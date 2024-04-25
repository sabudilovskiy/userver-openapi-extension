#pragma once
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/meta_request.hpp>
#include <uopenapi/http/response.hpp>
#include <uopenapi/http/response_info.hpp>
#include <uopenapi/http/response_serializator/serializator.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <userver/formats/json.hpp>

namespace uopenapi::http {

template <typename T>
response_info serialize_response_info(const T& t) {
    response_info res;
    static_assert(count_bodies<T>() <= 1, "maximum one body in request");
    auto visit_all = [&]<typename Info, typename F>(const F& f) {
        constexpr source_type st = field_source<T, Info::name>;
        reflective::is_validate_result auto validateResult = [&]() {
            if constexpr (utils::is_optional<F>) {
                if (utils::optional_getter<F>::has_value(f)) {
                    return reflective::field_call_validate<T, Info::name>(
                        utils::optional_getter<F>::value(f));
                } else {
                    // TODO: fix this
                    return reflective::validate_result::ok();
                }
            } else {
                return reflective::field_call_validate<T, Info::name>(f);
            }
        }();
        if (validateResult.has_error()) {
            throw utils::formatted_exception(
                "Failed serialize {} field. Error: [{}]", to_string_view(st),
                validateResult.error_message());
        }
        using serializator = response_serializator<F, st>;
        serializator::serialize(f, res, Info::name.AsStringView());
    };
    pfr_extension::for_each_named_field(t, visit_all);
    return res;
}

template <typename T, std::size_t Code>
response_info serialize_response_info(const response<T, Code>& resp) {
    auto res = serialize_response_info(resp.value);
    res.status_code = resp.code;
    return res;
}
}  // namespace uopenapi::http
