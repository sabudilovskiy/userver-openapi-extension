#pragma once
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/validate_result.hpp>
#include <uopenapi/utils/nttp_adl.hpp>

namespace uopenapi::reflective {
template <typename T, utils::ce::string name, typename F>
concept field_exist_validate =
    requires(const F& f) { validate(f, requirements_field<T, name>); };

template <typename T, utils::ce::string name, typename F>
concept field_exist_nttp_validate =
requires(const F& f) { validate(f, utils::create_nttp_adl<requirements_field<T, name>>()); };

template <typename T, none_requirements req>
validate_result validate(const T&, utils::nttp_adl<none_requirements, req>) {
    return validate_result::ok();
}

template <typename T, utils::ce::string name, typename F>
is_validate_result auto field_call_validate(const F& f) {
    if constexpr (nttp_requirements_field<T, name> && field_exist_nttp_validate<T, name, F>) {
        return validate(f, utils::create_nttp_adl<requirements_field<T, name>>());
    }
    else if constexpr (nttp_requirements_field<T, name> && !field_exist_nttp_validate<T, name, F>){
        static_assert(std::is_same_v<F, void> || std::is_same_v<requirements_field_t<T, name>, void>, "не найдена перегрузка с передачей как nttp");
    }
    else if constexpr (field_exist_validate<T, name, F>)
    {
        return validate(f, requirements_field<T, name>);
    }
    else {
        static_assert(
                std::is_same_v<F, void> || std::is_same_v<requirements_field_t<T, name>, void>,
            "Не найдена перегрузка validate с передачей requirements как "
            "нешаблонного аргумента."
            "Это возможно в двух случаях: "
            "1) У requirements отсутствует constexpr. Добавьте его. Если "
            "используете макрос, то правильный макрос: REQUIREMENTS_CE_UOPENAPI"
            "2) Перегрузка не видна в месте инстанцирования call_validate() "
            "или не существует вообще");
    }
}

}  // namespace uopenapi::reflective