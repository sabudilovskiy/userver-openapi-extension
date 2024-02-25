#pragma once
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/reflective/requirements/string/string_requirements.hpp>

namespace uopenapi::reflective{
    constexpr utils::constexpr_string kDateTime = "datetime";
    template <>
    struct string_validator<kDateTime> {
        static void validate(std::string_view str);
    };
}