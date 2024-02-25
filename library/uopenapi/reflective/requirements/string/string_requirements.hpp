#pragma once

#include <optional>
#include <uopenapi/reflective/requirements/requirements.hpp>
#include <uopenapi/utils/constexpr_optional.hpp>
#include <uopenapi/utils/formatted_exception.hpp>

namespace uopenapi::reflective{
    struct basic_string_requirements{
        std::optional<std::size_t>& min_length;
        std::optional<std::size_t>& max_length;
        std::optional<std::string>& pattern;
    };

    void validate(std::string_view str, basic_string_requirements requirements);

    template <utils::constexpr_string Format = "">
    struct string_requirements{
        std::optional<std::size_t> min_length;
        std::optional<std::size_t> max_length;
        std::optional<std::string> pattern;
        static constexpr auto format = Format;
    };

    template <utils::constexpr_string Format>
    struct string_validator{
        //must have static validate(std::string_view)
    };

    template <>
    struct string_validator<"">{
        //must have static validate(std::string_view)
    };

    template <utils::constexpr_string Format = "">
    void validate(std::string_view str, string_requirements<Format> requirements){
        validate(str, basic_string_requirements{
            .min_length = requirements.min_length,
            .max_length = requirements.max_length,
            .pattern = requirements.pattern
        });
        string_validator<Format>::validate(str);
    }
}