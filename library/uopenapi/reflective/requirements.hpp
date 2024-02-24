#pragma once
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::reflective{

    struct none_requirements{};
    template <typename T, utils::constexpr_string>
    auto requirements = none_requirements{};

    template<typename T>
    void validate(T& t, none_requirements){
        //noop, must throw if error
    }

    struct Test{
        int b;
    };

    struct integer_requirements{
        std::optional<std::int32_t> min;
        std::optional<std::int32_t> max;
    };

#define REQUIREMENTS_UOPENAPI(TYPE, FIELD) template<> auto requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>

    REQUIREMENTS_UOPENAPI(Test, b) = integer_requirements{
            .min = 1,
            .max = 2
    };

//    template<>
//    auto requirements<Test, "b"> = integer_requirements{
//        .min = 1,
//        .max = 2
//    };


}