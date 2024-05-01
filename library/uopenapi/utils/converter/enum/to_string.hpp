#pragma once
#include <string>
#include <type_traits>
#include <uopenapi/enum_helpers/string_cast.hpp>
#include <uopenapi/reflective/schema/schema_type_name.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>

namespace uopenapi::utils {
template <typename Enum>
requires enum_helpers::has_introspector<Enum>
struct converter<Enum, std::string> {
    static constexpr auto type = convert_type::strong;

    static std::string convert(Enum value) {
        auto stringView = enum_helpers::enum_to_string_view(value);
        if (!stringView) {
            auto underlying_value = (std::underlying_type_t<Enum>) value;
            throw utils::formatted_exception(
                "Try to convert corrupted value: [{}] which type is [{}]",
                underlying_value, reflective::schema_type_name<Enum>());
        }
        return std::string{*stringView};
    }
};
template <typename Enum>
requires enum_helpers::has_introspector<Enum>
struct converter<Enum, std::string_view> {
    static constexpr auto type = convert_type::strong;

    static std::string_view convert(Enum value) {
        auto stringView = enum_helpers::enum_to_string_view(value);
        if (!stringView) {
            auto underlying_value = (std::underlying_type_t<Enum>) value;
            throw utils::formatted_exception(
                "Try to convert corrupted value: [{}] which type is [{}]",
                underlying_value, reflective::schema_type_name<Enum>());
        }
        return *stringView;
    }
};
}  // namespace uopenapi::utils
