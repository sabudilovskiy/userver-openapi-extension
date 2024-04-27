#pragma once
#include <string_view>
#include <type_traits>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/http/request_parser/query/common.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <vector>

namespace uopenapi::http {
template <>
struct request_parser<std::string, source_type::query> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            throw details::missing_queries(fieldName);
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return queries[0];
    }
};

template <>
struct request_parser<std::string_view, source_type::query> {
    static std::string parse(const http::request_info& requestInfo,
                             std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            throw details::missing_queries(fieldName);
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return queries[0];
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string>
struct request_parser<Field, source_type::query> {
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return queries[0];
    }
};

template <typename Field>
requires utils::opt_underlying_of<Field, std::string_view>
struct request_parser<Field, source_type::query> {
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return std::string_view{queries[0]};
    }
};
}  // namespace uopenapi::http
