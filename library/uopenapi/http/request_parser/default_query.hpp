#pragma once
#include <string_view>
#include <uopenapi/http/request_info.hpp>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <vector>

namespace uopenapi::http {
template <typename F>
struct request_parser<std::vector<F>, source_type::query> {
    using Field = std::vector<F>;
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            return {};
        }
        auto& queries = it->second;
        return utils::converter<std::vector<std::string>, Field>::convert(
            queries);
    }
};

template <typename Field>
struct request_parser<Field, source_type::query> {
    using raw_type = std::conditional_t<utils::is_optional<Field>,
                                        utils::optional_getter_t<Field>, Field>;
    static void assert_quantity(const std::vector<std::string>& queries,
                                std::string_view fieldName) {
        if (queries.size() != 1) {
            throw utils::formatted_exception("Two more queries with name: [{}]",
                                             fieldName);
        }
    }
    static Field missing(std::string_view fieldName) {
        if constexpr (utils::is_optional<Field>) {
            return utils::optional_getter<Field>::make_none();
        } else {
            throw utils::formatted_exception(
                "Not founded queries with name: [{}]", fieldName);
        }
    }
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& q = requestInfo.queries;
        auto it = q.find(std::string{fieldName});
        if (it == q.end()) {
            return missing(fieldName);
        }
        auto& queries = it->second;
        assert_quantity(queries, fieldName);
        return utils::converter<std::string, raw_type>::convert(queries[0]);
    }
};
}  // namespace uopenapi::http
