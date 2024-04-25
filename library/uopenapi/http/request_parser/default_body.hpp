//
// Created by sabudilovskiy on 3/12/24.
//

#pragma once
#include <string_view>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename Field>
struct request_parser<Field, source_type::body_JSON> {
    static Field missing(std::string_view fieldName) {
        if constexpr (utils::is_optional<Field>) {
            return utils::optional_getter<Field>::make_none();
        } else {
            throw utils::formatted_exception("Not founded body. Name field: [{}]",
                                             fieldName);
        }
    }
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto body = requestInfo.body;
        if (body.empty()) {
            return missing(fieldName);
        }
        auto json = userver::formats::json::FromString(requestInfo.body);
        return json.As<Field>();
    }
};
}  // namespace uopenapi::http
