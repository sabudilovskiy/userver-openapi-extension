#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>
#include <mock_converters.hpp>

using namespace uopenapi::http;

inline namespace tests_path_http_schema{
    struct TestReqBody{
        some_enum first;
        some_enum second;
    };
    struct TestReq{
        some_enum header_enum;
        some_enum cookie_enum;
        std::vector<some_enum> query_items;
        TestReqBody body;
    };
    struct TestRespBody{
        some_enum first;
        some_enum second;
    };
    struct TestResp{
        some_enum header_enum;
        TestRespBody body;
    };
}
UOPENAPI_SOURCE_TYPE(TestReq, header_enum, header);
UOPENAPI_SOURCE_TYPE(TestReq, query_items, query);
UOPENAPI_SOURCE_TYPE(TestReq, cookie_enum, cookie);

UOPENAPI_SOURCE_TYPE(TestResp, header_enum, header);

UTEST(openapi_schema, BasicPath){
uopenapi::reflective::schema schema;
std::string path = "/login";
std::string method = "POST";
handler_info handlerInfo{
    .path = path,
    .method = method,
};
append_path<TestReq, uopenapi::http::response<TestResp, 200>>(schema, handlerInfo);
auto str = ToString(schema.v.ExtractValue());
EXPECT_EQ(str, UOPENAPI_RAW_STRING(R"(
paths:
  /login:
    post:
      description: tests_path_http_schema.TestReq
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
              $ref: "#/components/schemas/tests_path_http_schema.TestReqBody"
      responses:
        200:
          $ref: "#/components/responses/tests_path_http_schema.TestResp"
components:
  schemas:
    some_enum:
      type: string
      enum:
        - A
        - B
        - C
        - D
    tests_path_http_schema.TestReqBody:
      type: object
      properties:
        first:
          $ref: "#/components/schemas/some_enum"
        second:
          $ref: "#/components/schemas/some_enum"
      required:
        - first
        - second
    tests_path_http_schema.TestRespBody:
      type: object
      properties:
        first:
          $ref: "#/components/schemas/some_enum"
        second:
          $ref: "#/components/schemas/some_enum"
      required:
        - first
        - second
  responses:
    tests_path_http_schema.TestResp:
      description: ""
      headers:
        header_enum:
          schema:
            $ref: "#/components/schemas/some_enum"
            required: true
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/tests_path_http_schema.TestRespBody"
)")) << str;
}

