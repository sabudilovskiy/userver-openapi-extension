#include <uopenapi/http/openapi_descriptor.h>

#include <mock_converters.hpp>
#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::http;

inline namespace tests_request_http_schema {
struct TestBody {
    some_enum first;
    some_enum second;
};
struct TestReq {
    some_enum header_enum;
    some_enum cookie_enum;
    std::vector<some_enum> query_items;
    TestBody body;
};
}  // namespace tests_request_http_schema
UOPENAPI_SOURCE_TYPE(TestReq, query_items, query);
UOPENAPI_SOURCE_TYPE(TestReq, header_enum, header);
UOPENAPI_SOURCE_TYPE(TestReq, cookie_enum, cookie);

UTEST(openapi_schema, BasicRequest) {
    uopenapi::reflective::schema schema;
    auto sv_root = uopenapi::reflective::schema_view::from_schema(schema);
    auto path = sv_root.root["paths"]["/login"]["post"];
    auto sv_path = sv_root.from_node(path);
    details::append_request<TestReq>(sv_path);
    auto str = ToString(schema.v.ExtractValue());
    EXPECT_EQ(str, UOPENAPI_RAW_STRING(R"(
paths:
  /login:
    post:
      description: tests_request_http_schema.TestReq
      parameters:
        - in: header
          name: header_enum
          required: false
          schema:
            $ref: "#/components/schemas/some_enum"
        - in: cookie
          name: cookie_enum
          required: false
          schema:
            $ref: "#/components/schemas/some_enum"
        - in: query
          name: query_items
          required: false
          schema:
            type: array
            items:
              $ref: "#/components/schemas/some_enum"
      requestBody:
        required: true
        content:
          application/json:
            schema:
              $ref: "#/components/schemas/tests_request_http_schema.TestBody"
components:
  schemas:
    some_enum:
      type: string
      enum:
        - A
        - B
        - C
        - D
    tests_request_http_schema.TestBody:
      type: object
      properties:
        first:
          $ref: "#/components/schemas/some_enum"
        second:
          $ref: "#/components/schemas/some_enum"
      required:
        - first
        - second
)"));
}
