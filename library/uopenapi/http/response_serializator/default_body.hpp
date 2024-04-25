#pragma once
#include <uopenapi/http/response_info.hpp>
#include <uopenapi/http/response_serializator/serializator.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename T>
requires utils::is_optional<T>
struct response_serializator<T, source_type::body_JSON> {
    static void serialize(const T& t, response_info& r_info,
                          std::string_view fieldName) {
        if (!r_info.body.empty()) {
            throw utils::formatted_exception(
                "Detected multiply bodies. Second body: [{}]", fieldName);
        }
        if (!utils::optional_getter<T>::has_value(t)) {
            return;
        }
        r_info.content_type = userver::http::content_type::kApplicationJson;
        auto& value = utils::optional_getter<T>::value(t);
        r_info.body = ToString(
            userver::formats::json::ValueBuilder{value}.ExtractValue());
    }
};

template <typename T>
requires(!utils::is_optional<T>)
struct response_serializator<T, source_type::body_JSON> {
    static void serialize(const T& t, response_info& r_info,
                          std::string_view fieldName) {
        if (!r_info.body.empty()) {
            throw utils::formatted_exception(
                "Detected multiply bodies. Second body: [{}]", fieldName);
        }
        r_info.content_type = userver::http::content_type::kApplicationJson;
        r_info.body =
            ToString(userver::formats::json::ValueBuilder{t}.ExtractValue());
    }
};
}  // namespace uopenapi::http
