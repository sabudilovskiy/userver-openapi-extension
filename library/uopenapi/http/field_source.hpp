#pragma once
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/utils/check_field.hpp>
#include <uopenapi/http/source_type.hpp>

namespace uopenapi::http{
    template <typename T, utils::ce::string name>
    constexpr source_type field_source = source_type::query;

    template <typename T>
    constexpr source_type field_source<T, "body"> = source_type::body;
}

#ifdef UOPENAPI_CREATE_MACROS
#define UOPENAPI_SOURCE_TYPE(TYPE, FIELD, SOURCE_TYPE) \
namespace uopenapi::http{                                                            \
template <>                                                            \
constexpr source_type field_source<TYPE,  UOPENAPI_FIELD(TYPE, FIELD)> = uopenapi::http::source_type::SOURCE_TYPE;                        \
}                                                      \

#endif