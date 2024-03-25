#pragma once

#define MOCK_OPENAPI_NAME(Type)                   \
    namespace uopenapi::reflective {              \
    template <>                                   \
    inline std::string schema_type_name<Type>() { \
        return #Type;                             \
    }                                             \
    }