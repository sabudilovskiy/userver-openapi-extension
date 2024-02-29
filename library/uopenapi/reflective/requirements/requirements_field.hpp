#pragma once
#include "uopenapi/utils/constexpr_string.hpp"
#include "uopenapi/pfr_extension/get_index.hpp"
#include <uopenapi/reflective/requirements/none_requirements.hpp>
#include <uopenapi/utils/check_nttp.hpp>

namespace uopenapi::reflective{

    /* Данная переменная будет использоваться для вызова соответствующей функции validate
     * validate(t, requrements) - будет вызвана для тех requirements, которые не могут быть переданы как nttp
     * validate<requrements>(t) - будет вызвана, если можно передать как nttp
     * Использование nttp позволяет сгенерировать разные и более эффективные функции валидации
     * validate должна возвращать validate_result
     * validate будет вызвана после парса и перед сериализацией
     */
    template <typename T, utils::ce::string name>
    constexpr auto requirements_field = none_requirements{};

    template <typename T, utils::ce::string name>
    using requirements_field_t = decltype(requirements_field<T, name>);

    template <typename T, utils::ce::string name>
    constexpr bool trivial_requirements_field = std::is_same_v<requirements_field_t<T, name>, none_requirements>;

    template <typename T, utils::ce::string name>
    concept nttp_requirements_field = requires {
        utils::check_nttp<requirements_field<T, name>>();
    };
}

#ifdef UOPENAPI_CREATE_MACROS
#define REQUIREMENTS_UOPENAPI(TYPE, FIELD) template<> auto ::uopenapi::reflective::requirements_field<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#define REQUIREMENTS_CE_UOPENAPI(TYPE, FIELD) template<> constexpr auto ::uopenapi::reflective::requirements_field<TYPE, (decltype(std::declval<TYPE>().FIELD, #FIELD))#FIELD>
#endif