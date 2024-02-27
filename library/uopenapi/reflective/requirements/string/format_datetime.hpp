#pragma once
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/reflective/requirements/string/string_requirements.hpp>

namespace uopenapi::reflective{
    constexpr utils::ce::string kDateTime = "datetime";
    template <>
    struct string_validator<kDateTime> {
        static validate_result validate(std::string_view str);
    };
}