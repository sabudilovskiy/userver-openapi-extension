#pragma once

#include <optional>
#include <uopenapi/utils/constexpr_optional.hpp>
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/reflective/validate_result.hpp>

namespace uopenapi::reflective{

    bool check_pattern(std::string_view str, std::string_view pattern);

    template <utils::ce::string Format = "">
    struct string_requirements{
        utils::ce::optional<std::size_t> min_length;
        utils::ce::optional<std::size_t> max_length;
        utils::ce::optional<utils::ce::string> pattern;
        static constexpr auto format = Format;
    };

    template <utils::ce::string Format>
    struct string_validator{
        //must have static validate_result validate(std::string_view)
    };

    template <>
    struct string_validator<"">{
        static validate_result validate(std::string_view){
            return validate_result::ok();
        }
    };

    template <string_requirements req>
    validate_result validate(std::string_view str){
        if (req.min_length && str.size() < *req.min_length){
             return validate_result::error("str.size: {}, min_length: {}", str.size(), *req.max_length);
        }
        if (req.max_length && str.size() > *req.max_length){
             return validate_result::error("str.size: {}, min_length: {}", str.size(), *req.max_length);
        }
        if (req.pattern && !check_pattern(str, *req.pattern)){
            return validate_result::error("str: {}, doesn't match with: {}", str, *req.pattern);
        }
        return string_validator<req.format>::validate(str);
    }
}