#include <userver/utest/utest.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <vector>
#include <unordered_map>
#include <string>

namespace {
    struct SecretStruct{
        std::string c;
        int a; //min: 1, max: 2
        std::string b;
    };
    struct SecretStruct2{
        std::string d;
        int a; //min: 1, max: 2
        std::string b;
    };
}

UTEST(Openapi_json_Parse, SomeStruct){
//    EXPECT_EQ(uopenapi::reflective::schema_type_name<std::vector<SecretStruct>>(), "");
//    EXPECT_EQ(std::string{uopenapi::utils::anonymous_ns}, "");
    static_assert(uopenapi::utils::is_specialisation<std::vector<int>, "std::vector">(), "test this thit");
    static_assert(uopenapi::utils::is_specialisation<std::map<int, int>, "std::map">(), "test this thit");
}

