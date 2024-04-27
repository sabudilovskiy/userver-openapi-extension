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

template <>
struct request_parser<std::string, source_type::header> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            throw details::missing_header(fieldName);
        }
        auto& header = it->second;
        return header;
    }
};

template <>
struct request_parser<std::string_view, source_type::header> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            throw details::missing_header(fieldName);
        }
        auto& header = it->second;
        return header;
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string>
struct request_parser<Field, source_type::header> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& header = it->second;
        return header;
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string_view>
struct request_parser<Field, source_type::header> {
    static std::string_view parse(const http::request_info& requestInfo,
                                  std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& header = it->second;
        return header;
    }
};

}  // namespace uopenapi::http
