#include <userver/utest/utest.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/json/reflective_parse.hpp>
#include <uopenapi/json/reflectie_serialize.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <uopenapi/reflective/requirements/requirements.hpp>
#include <uopenapi/reflective/requirements/string/format_datetime.hpp>
#include <uopenapi/reflective/requirements/integer/integer_requirements.hpp>

namespace {
    struct SecretStruct{
        int a; //min: 1, max: 2
        std::string b;
        std::optional<std::string> c;
    };
}

REQUIREMENTS_CE_UOPENAPI(SecretStruct, a) = uopenapi::reflective::integer_requirements<int>{
    .minimum = 0,
    .exclusive_minimum = true
};

REQUIREMENTS_CE_UOPENAPI(SecretStruct, b) = uopenapi::reflective::string_requirements<kDateTime>{};

UTEST(Openapi_json_Parse, SomeStruct){
    userver::formats::json::ValueBuilder json{userver::formats::common::Type::kObject};
    json["a"] = 5;
    json["b"] = "2024-02-25T10:50:44Z";
    json["c"] = "f";
    auto j = json.ExtractValue();
    auto secret_struct = j.As<SecretStruct>();
    ASSERT_EQ (secret_struct.a,1);
    ASSERT_EQ (secret_struct.b,"2002.");
    ASSERT_EQ (secret_struct.c, "f");
}

UTEST(Openapi_json_Serialize, SomeStruct){
    SecretStruct some_struct;
    some_struct.a = 5;
    some_struct.b = "2024-02-25T10:50:44Z";
    some_struct.c = "f";
    auto json = userver::formats::json::ValueBuilder{some_struct}.ExtractValue();
    ASSERT_EQ(json["a"].As<int>(), 5);
    ASSERT_EQ(json["b"].As<std::string>(), "2024-02-25T10:50:44Z");
    ASSERT_EQ(json["c"].As<std::optional<std::string>>(), "f");
}