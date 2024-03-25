#pragma once
#include <uopenapi/http/response_info.hpp>
#include <uopenapi/http/response_serializator/serializator.hpp>
#include <uopenapi/utils/converter/string/string.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename T>
requires(!utils::is_optional<T>)
struct response_serializator<T, source_type::header> {
    static void serialize(const T& t, response_info& r_info,
                          std::string_view name) {
        using converter = utils::converter<T, std::string>;
        auto [it, emplaced] =
            r_info.headers.emplace(name, converter::convert(t));
        if (!emplaced) {
            throw utils::formatted_exception(
                "Detected duplicated field in headers: [{}]", name);
        }
    }
};
template <typename T>
requires(utils::is_optional<T>)
struct response_serializator<T, source_type::header> {
    static void serialize(const T& t, response_info& r_info,
                          std::string_view name) {
        using converter =
            utils::converter<utils::optional_getter_t<T>, std::string>;
        using getter = utils::optional_getter<T>;
        if (!getter::has_value(t)) {
            return;
        }
        auto [_, emplaced] =
            r_info.headers.emplace(name, converter::convert(getter::value(t)));
        if (!emplaced) {
            throw utils::formatted_exception(
                "Detected duplicated field in headers: [{}]", name);
        }
    }
};
}  // namespace uopenapi::http
