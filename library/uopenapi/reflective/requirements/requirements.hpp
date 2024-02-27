#pragma once
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/reflective/requirements/validate_result.hpp>

namespace uopenapi::reflective{

    struct none_requirements{};
    /* Данная переменная будет использоваться для вызова соответствующей функции validate
     * validate(t, requrements) - будет вызвана для тех requirements, которые не могут быть переданы как nttp
     * validate<requrements>(t) - будет вызвана, если можно передать как nttp
     * Использование nttp позволяет сгенерировать разные и более эффективные функции валидации
     * validate должна возвращать validate_result
     * validate будет вызвана после парса и перед сериализацией
     */
    template <typename T, utils::ce::string name>
    constexpr auto requirements = none_requirements{};

    namespace details{
        template <auto>
        consteval void check_nttp(){}
    }

    template <typename T, utils::ce::string name>
    concept requirements_nttp = requires {
        details::check_nttp<requirements<T, name>>();
    };

    template <typename T, utils::ce::string name, typename F>
    concept exist_validate= requires (const F& f){
        validate(f, requirements<T,name>);
    };

    template <typename T, utils::ce::string name, typename F>
    auto call_validate(const F& f){
        if constexpr (requirements_nttp<T, name>){
            return validate<requirements<T,name>>(f);
        }
        else if constexpr (exist_validate<T, name, F>){
            return validate(f, requirements<T,name>);
        }
        else {
            static_assert (exist_validate<T, name, F>,
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
}

#ifdef UOPENAPI_CREATE_MACROS
#define REQUIREMENTS_UOPENAPI(TYPE, FIELD) template<> auto ::uopenapi::reflective::requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#define REQUIREMENTS_CE_UOPENAPI(TYPE, FIELD) template<> constexpr auto ::uopenapi::reflective::requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#endif