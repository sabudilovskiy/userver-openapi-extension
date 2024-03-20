#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <unordered_map>
#include <string>
#include <raw_string.hpp>

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
    EXPECT_EQ(result, UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    Data:
      type: object
      properties:
        a:
          type: integer
          format: int32
          minimum: 1
          maximum: 10
        b:
          type: integer
          format: int32
          minimum: 1
          exclusiveMinimum: true
        c:
          type: integer
          format: int32
        d:
          type: integer
          format: int32
        test:
          type: string
          format: date-time
          pattern: f$
      required:
        - a
        - b
        - c
        - d
        - test
)"));
}

