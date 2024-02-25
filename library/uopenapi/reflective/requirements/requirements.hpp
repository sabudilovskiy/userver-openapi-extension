#pragma once
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::reflective{

    struct none_requirements{};

    //специализируйте переменную для того, чтобы задать ограничения для поля
    template <typename T, utils::constexpr_string>
    auto requirements = none_requirements{};

    //функция вызовется после парса поля и проверит, что значение корректное
    //пользовательская функция должна использовать throw для сигнализации в случае ошибки
    template<typename T>
    void validate(T&, none_requirements){
        //noop, must throw if error
    }
}

#ifdef UOPENAPI_CREATE_MACROS
#define REQUIREMENTS_UOPENAPI(TYPE, FIELD) template<> auto ::uopenapi::reflective::requirements<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#endif