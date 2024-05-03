#pragma once
#include <string>
#include <uopenapi/utils/converter/converter.hpp>

namespace uopenapi::utils {
template <>
struct converter<std::string, std::string> {
    static constexpr auto type = convert_type::strong;
    static std::string convert(const std::string& source) { return source; };
};
template <>
struct converter<std::string_view, std::string> {
    static constexpr auto type = convert_type::strong;
    static std::string convert(std::string_view source) {
        return std::string{source};
    };
};
}  // namespace uopenapi::utils
