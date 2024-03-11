#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <unordered_map>
#include <string>

using namespace uopenapi::reflective;

struct SecretStruct{
    std::string b;
};

struct Data{
    int a;
    int b;
    int c;
    int d;
    std::string test;
};

REQUIREMENTS_CE_UOPENAPI(Data, a) = number_requirements<int>{
    .minimum  = 1,
    .maximum = 10
};

REQUIREMENTS_CE_UOPENAPI(Data, b) = number_requirements<int>{
        .minimum  = 1,
        .exclusive_minimum = true
};


REQUIREMENTS_CE_UOPENAPI(Data, test) = string_requirements<"date-time">{
    .pattern = "f$"
};


UTEST(Openapi_json_Parse, SomeStruct){
    schema s;
    auto view = schema_view::from_schema(s);
    schema_appender<Data, none_requirements>::append<none_requirements{}>(view);
    auto result = ToString(s.v.ExtractValue());
    EXPECT_EQ(result, "");
}

