#pragma once
#include <string>
#include <type_traits>
#include <uopenapi/enum/string_cast.hpp>
#include <uopenapi/reflective/schema/schema_type_name.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>

namespace uopenapi::utils {
template <typename Enum>
requires std::is_enum_v<Enum> && HasIntrospector<Enum>
struct converter<std::string, Enum> {
    static Enum convert(const std::string& str) {
        auto value = enum_from_string_view<Enum>(str);
        if (!value) {
            throw utils::formatted_exception(
                "Try to convert unknown value: [{}] to [{}]", str,
                reflective::schema_type_name<Enum>());
        }
        return *value;
    }
};
template <typename Enum>
requires std::is_enum_v<Enum> && HasIntrospector<Enum>
struct converter<std::string_view, Enum> {
    static Enum convert(std::string_view str) {
        auto value = enum_from_string_view<Enum>(str);
        if (!value) {
            throw utils::formatted_exception(
                "Try to convert unknown value: [{}] to [{}]", str,
                reflective::schema_type_name<Enum>());
        }
        return *value;
    }
};
}  // namespace uopenapi::utils
