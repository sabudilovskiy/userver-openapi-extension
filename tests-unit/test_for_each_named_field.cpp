#include <userver/utest/utest.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/json/reflective_parse.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <uopenapi/utils/constexpr_string.hpp>
#include <uopenapi/reflective/requirements.hpp>


namespace {
    struct SecretStruct{
        int a; //min: 1, max: 2
        std::string b;
        std::optional<std::string> c;
    };
}

UTEST(Openapi_json_Parse, SomeStruct){
    userver::formats::json::ValueBuilder json{userver::formats::common::Type::kObject};
    json["a"] = 1;
    json["b"] = "test";
    json["c"] = "f";
    auto j = json.ExtractValue();
    auto secret_struct = j.As<SecretStruct>();
    ASSERT_EQ (secret_struct.a,1);
    ASSERT_EQ (secret_struct.b,"test");
    ASSERT_EQ (secret_struct.c, "f");
}