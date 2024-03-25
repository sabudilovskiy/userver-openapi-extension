#pragma once

#ifdef UOPENAPI_CREATE_MACROS
#define UOPENAPI_FIELD(TYPE, FIELD) \
    (decltype(std::declval<TYPE>().FIELD, #FIELD)) #FIELD
#endif