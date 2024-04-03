#pragma once
#include <string>
#include <string_view>
#include <type_traits>
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/utils/from_string.hpp>

namespace uopenapi::utils {
template <typename Number> requires std::is_arithmetic_v<Number>
struct converter<std::string, Number> {
    static Number convert(const std::string& str) {
        return userver::utils::FromString<Number>(str);;
    }
};
template <typename Number> requires std::is_arithmetic_v<Number>
struct converter<std::string_view, Number> {
    static Number convert(std::string_view str) {
        return userver::utils::FromString<Number>(str);;
    }
};
}  // namespace uopenapi::utils
