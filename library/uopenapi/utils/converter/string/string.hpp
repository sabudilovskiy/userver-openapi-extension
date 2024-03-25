#pragma once
#include <string>
#include <uopenapi/utils/converter/converter.hpp>

namespace uopenapi::utils {
template <>
struct converter<std::string, std::string> {
    using source_type = std::string;
    using result_type = std::string;
    static result_type convert(const source_type& source) { return source; };
};
template <>
struct converter<std::string_view, std::string> {
    using source_type = std::string_view;
    using result_type = std::string;
    static result_type convert(source_type source) {
        return result_type{source};
    };
};
}  // namespace uopenapi::utils