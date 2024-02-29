#pragma once
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/validate_result.hpp>

namespace uopenapi::reflective{
    template <typename T, utils::ce::string name, typename F>
    concept field_exist_validate = requires (const F& f){
        validate(f, requirements_field<T,name>);
    };

    template <typename T, utils::ce::string name, typename F>
    is_validate_result auto field_call_validate(const F& f){
        if constexpr (nttp_requirements_field<T, name>){
            return validate<requirements_field<T,name>>(f);
        }
        else if constexpr (field_exist_validate<T, name, F>){
            return validate(f, requirements_field<T,name>);
        }
        else {
            static_assert (field_exist_validate<T, name, F>,
                           "Не найдена перегрузка validate с передачей requirements как нешаблонного аргумента."
                           "Это возможно в двух случаях: "
                           "1) У requirements отсутствует constexpr. Добавьте его. Если используете макрос, то правильный макрос: REQUIREMENTS_CE_UOPENAPI"
                           "2) Перегрузка не видна в месте инстанцирования call_validate() или не существует вообще");
        }
    }

    template<none_requirements, typename T>
    validate_result validate(T&){
        return validate_result::ok();
    }
    template<typename T>
    validate_result validate(T&){
        return validate_result::ok();
    }
}