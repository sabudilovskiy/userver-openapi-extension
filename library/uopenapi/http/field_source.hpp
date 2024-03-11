#pragma once
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/utils/check_field.hpp>

namespace uopenapi::http{

    enum struct source_type {
        body, query, header, cookie
    };
    template <typename T, utils::ce::string name>
    constexpr source_type field_source = source_type::body;
}

#ifdef UOPENAPI_CREATE_MACROS
#define UOPENAPI_SOURCE_TYPE(TYPE, FIELD, SOURCE_TYPE) \
namespace uopenapi::http{                                                            \
template <>                                                            \
constexpr source_type field_source<TYPE,  UOPENAPI_FIELD(TYPE, FIELD)> = uopenapi::http::source_type::SOURCE_TYPE;                        \
}                                                      \

#endif