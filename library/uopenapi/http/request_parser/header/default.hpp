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
struct request_parser<Field, source_type::header> {
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            throw details::missing_header(fieldName);
        }
        auto& header = it->second;
        return utils::converter<std::string, Field>::convert(header);
    }
};

template <typename Field>
requires(utils::is_optional<Field> &&
         utils::has_any_converter<std::string, utils::optional_getter_t<Field>>)
struct request_parser<Field, source_type::header> {
    using raw_type = utils::optional_getter_t<Field>;

    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& h = requestInfo.headers;
        auto it = h.find(std::string{fieldName});
        if (it == h.end()) {
            return utils::optional_getter<Field>::make_none();
        }
        auto& header = it->second;
        return utils::converter<std::string, raw_type>::convert(header);
    }
};

}  // namespace uopenapi::http
