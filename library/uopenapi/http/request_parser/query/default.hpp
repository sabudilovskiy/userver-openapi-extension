#pragma once
#include <string_view>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/http/request_parser/query/common.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <userver/formats/json/validate.hpp>
#include <vector>

namespace uopenapi::http {

template <typename Field>
requires(!utils::is_optional<Field> &&
         utils::has_any_converter<std::string, Field>)
struct request_parser<Field, source_type::query> {
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            throw details::missing_queries(fieldName);
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return utils::converter<std::string, Field>::convert(queries[0]);
    }
};

template <typename Field>
requires(utils::is_optional<Field> &&
         utils::has_any_converter<std::string, utils::optional_getter_t<Field>>)
struct request_parser<Field, source_type::query> {
    using raw_type = utils::optional_getter_t<Field>;

    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& queries = it->second;
        details::assert_queries_quantity(queries, fieldName);
        return utils::converter<std::string, raw_type>::convert(queries[0]);
    }
};
}  // namespace uopenapi::http
