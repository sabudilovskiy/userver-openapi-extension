#include <raw_string.hpp>
#include <string>
#include <unordered_map>
#include <uopenapi/all.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

struct SecretStruct {
    std::string b;
};

struct Data {
    int a;
    int b;
    int c;
    int d;
    std::string test;
    SecretStruct secret_struct;
};

UOPENAPI_CE_REQUIREMENTS(Data, a) = number_requirements<int>{.minimum = 1,
                                                             .maximum = 10};

UOPENAPI_CE_REQUIREMENTS(Data, b) = number_requirements<int>{
    .minimum = 1, .exclusive_minimum = true};

UOPENAPI_CE_REQUIREMENTS(Data, test) = string_requirements<"date-time">{
    .pattern = "f$"};

UTEST(Openapi_json_Parse, SomeStruct) {
    schema s;
    auto view = schema_view::from_schema(s);
    schema_appender<Data, none_requirements>::append(view, none_requirements{});
    auto result = ToString(s.v.ExtractValue());
    EXPECT_EQ(result, UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    Data:
      type: object
      additionalProperties: false
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
        secret_struct:
          $ref: "#/components/schemas/SecretStruct"
      required:
        - a
        - b
        - c
        - d
        - test
        - secret_struct
    SecretStruct:
      type: object
      additionalProperties: false
      properties:
        b:
          type: string
      required:
        - b
)")) << result;
}
