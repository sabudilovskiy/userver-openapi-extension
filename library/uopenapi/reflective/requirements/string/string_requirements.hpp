#pragma once

#include <optional>
#include <uopenapi/reflective/requirements/requirements.hpp>
#include <uopenapi/utils/constexpr_optional.hpp>
#include <uopenapi/utils/formatted_exception.hpp>

namespace uopenapi::reflective{

    void check_pattern(std::string_view str, std::string_view pattern);

    template <utils::ce::string Format = "">
    struct string_requirements{
        utils::ce::optional<std::size_t> min_length;
        utils::ce::optional<std::size_t> max_length;
        utils::ce::optional<utils::ce::string> pattern;
        static constexpr auto format = Format;
    };

    template <utils::ce::string Format>
    struct string_validator{
        //must have static validate(std::string_view)
    };

    template <>
    struct string_validator<"">{
        constexpr static void validate(std::string_view){
            //no-op
        }
    };

    template <string_requirements req>
    void validate(std::string_view str){
        if (req.min_length && str.size() < *req.min_length){
            throw utils::formated_exception("str.size: {}, min_length: {}", str.size(), *req.max_length);
        }
        if (req.max_length && str.size() > *req.max_length){
            throw utils::formated_exception("str.size: {}, min_length: {}", str.size(), *req.max_length);
        }
        string_validator<req.format>::validate(str);
        check_pattern(str, req.format.AsStringView());
    }
}