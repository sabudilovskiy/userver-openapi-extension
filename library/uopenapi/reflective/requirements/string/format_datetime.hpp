#pragma once
#include <uopenapi/reflective/requirements/string/string_requirements.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::reflective {
constexpr utils::ce::string kDateTime = "datetime";
template <>
struct string_validator<kDateTime> {
    static validate_result validate(std::string_view str);
};
}  // namespace uopenapi::reflective