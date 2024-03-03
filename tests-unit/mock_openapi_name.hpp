#pragma once


#define MOCK_OPENAPI_NAME(Type) namespace uopenapi::reflective{ \
template <> \
std::string schema_type_name<Type>(){ \
    return #Type; \
} \
}