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
requires utils::has_any_converter<std::vector<std::string>, std::vector<F>>
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
}  // namespace uopenapi::http
