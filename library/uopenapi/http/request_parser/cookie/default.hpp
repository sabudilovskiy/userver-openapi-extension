#pragma once
#include <string_view>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/header/common.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename Field>
requires(!utils::is_optional<Field> &&
         utils::has_any_converter<std::string, Field>)
struct request_parser<Field, source_type::cookie> {
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            throw details::missing_header(fieldName);
        }
        auto& cookie = it->second;
        return utils::converter<std::string, Field>::convert(cookie);
    }
};

template <typename Field>
requires(utils::is_optional<Field> &&
         utils::has_any_converter<std::string, utils::optional_getter_t<Field>>)
struct request_parser<Field, source_type::cookie> {
    using raw_type = utils::optional_getter_t<Field>;
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& cookie = it->second;
        return utils::converter<std::string, raw_type>::convert(cookie);
    }
};
}  // namespace uopenapi::http
