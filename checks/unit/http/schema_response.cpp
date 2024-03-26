#include <mock_converters.hpp>
#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::http;

namespace tests_response_http_schema {
struct TestBody {
    some_enum first;
    some_enum second;
};
struct TestReq {
    some_enum header_enum;
    TestBody body;
};
}  // namespace tests_response_http_schema

using tests_response_http_schema::TestReq;

UOPENAPI_SOURCE_TYPE(TestReq, header_enum, header);

UTEST(openapi_schema, BasicResponse) {
    uopenapi::reflective::schema schema;
    auto path_node = schema.v["paths"]["/test"];
    auto sv = uopenapi::reflective::schema_view{
        .root = schema.v,
        .cur_place = path_node,
    };
    details::append_response<TestReq, 200>(sv);
    auto str = ToString(schema.v.ExtractValue());
    EXPECT_EQ(str, UOPENAPI_RAW_STRING(R"(
paths:
  /test:
    200:
      $ref: "#/components/responses/tests_response_http_schema.TestReq"
components:
  responses:
    tests_response_http_schema.TestReq:
      description: ""
      headers:
        header_enum:
          schema:
            $ref: "#/components/schemas/some_enum"
            required: true
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/tests_response_http_schema.TestBody"
  schemas:
    some_enum:
      type: string
      enum:
        - A
        - B
        - C
        - D
    tests_response_http_schema.TestBody:
      type: object
      properties:
        first:
          $ref: "#/components/schemas/some_enum"
        second:
          $ref: "#/components/schemas/some_enum"
      required:
        - first
        - second
)")) << str;
}
