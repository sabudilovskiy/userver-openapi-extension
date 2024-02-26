#pragma once
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::reflective{

    struct none_requirements{};

    /* Данная переменная будет использоваться для вызова соответствующей функции validate
     * validate(t, requrements) - будет вызвана для тех requirements, которые не могут быть переданы как nttp
     * validate<requrements>(t) - будет вызвана, если можно передать как nttp
     * Использование nttp позволяет сгенерировать разные и более эффективные функции валидации
     * validate должна бросить exception, если валидация провалилась.
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
    void call_validate(const F& f){
        if constexpr (requirements_nttp<T, name>){
            validate<requirements<T,name>>(f);
        }
        else {
            validate(f, requirements<T,name>);
        }
    }

    template<none_requirements, typename T>
    void validate(T&){
        //noop, must throw if error
    }
}

#ifdef UOPENAPI_CREATE_MACROS
#define REQUIREMENTS_UOPENAPI(TYPE, FIELD) template<> auto ::uopenapi::reflective::requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#define REQUIREMENTS_CE_UOPENAPI(TYPE, FIELD) template<> constexpr auto ::uopenapi::reflective::requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#endif