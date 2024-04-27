#pragma once
#include <string_view>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/cookie/common.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {

template <>
struct request_parser<std::string, source_type::cookie> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            throw details::missing_cookie(fieldName);
        }
        auto& cookie = it->second;
        return cookie;
    }
};

template <>
struct request_parser<std::string_view, source_type::cookie> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            throw details::missing_cookie(fieldName);
        }
        auto& cookie = it->second;
        return cookie;
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string>
struct request_parser<Field, source_type::cookie> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& cookie = it->second;
        return cookie;
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string_view>
struct request_parser<Field, source_type::cookie> {
    static std::string_view parse(const http::request_info& requestInfo,
                                  std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& cookie = it->second;
        return cookie;
    }
};

}  // namespace uopenapi::http
