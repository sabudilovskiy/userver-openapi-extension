#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>
#include <mock_converters.hpp>

using namespace uopenapi::http;

inline namespace tests_request_http{
    struct TestBodyResp{
        some_enum first;
        some_enum second;
    };
    struct TestResp{
        some_enum header_enum;
        some_enum cookie_enum;
        TestBodyResp body;
    };
}

UOPENAPI_SOURCE_TYPE(TestResp, header_enum, header);
UOPENAPI_SOURCE_TYPE(TestResp, cookie_enum, cookie);

UTEST(openapi_validates, HttpSerializeResponseInfo){
   TestResp resp{
       .header_enum = some_enum::C,
       .cookie_enum = some_enum::D,
       .body = TestBodyResp{
           .first = some_enum::A,
           .second = some_enum::B
       },
   };
    auto resp_info = serialize_response_info(resp);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.content_type, userver::http::content_type::kApplicationJson);
    EXPECT_EQ(resp_info.body, R"({"first":"A","second":"B"})");
    EXPECT_EQ(resp_info.headers,
              (RespInfo::headers_map {{"header_enum", "C"}}));
    EXPECT_EQ(resp_info.cookies,
              (RespInfo::cookies_map {{"cookie_enum", "D"}}));
}
