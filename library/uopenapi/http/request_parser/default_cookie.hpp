#pragma once
#include <string_view>
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename Field>
struct request_parser<Field, source_type::cookie> {
    using raw_type = std::conditional_t<utils::is_optional<Field>,
                                        utils::optional_getter_t<Field>, Field>;
    static Field missing(std::string_view fieldName) {
        if constexpr (utils::is_optional<Field>) {
            return utils::optional_getter<Field>::make_none();
        } else {
            throw utils::formatted_exception(
                "Not founded cookie with name: [{}]", fieldName);
        }
    }
    static Field parse(const http::request_info& requestInfo,
                       std::string_view fieldName) {
        auto& c = requestInfo.cookies;
        auto it = c.find(std::string{fieldName});
        if (it == c.end()) {
            return missing(fieldName);
        }
        auto& cookie = it->second;
        return utils::converter<std::string, raw_type>::convert(cookie);
    }
};
}  // namespace uopenapi::http
