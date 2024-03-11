#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>
#include <mock_converters.hpp>

using namespace uopenapi::http;

inline namespace tests_request_http{
    struct TestBody{
        some_enum first;
        some_enum second;
    };
    struct TestReq{
        some_enum header_enum;
        some_enum cookie_enum;
        std::vector<some_enum> query_items;
        TestBody body;
    };
}
UOPENAPI_SOURCE_TYPE(TestReq, query_items, query);
UOPENAPI_SOURCE_TYPE(TestReq, header_enum, header);
UOPENAPI_SOURCE_TYPE(TestReq, cookie_enum, cookie);

UTEST(openapi_validates, HttpParseRequestInfo){
    test_request req{
        .cookies = {{"cookie_enum", "B"}},
        .headers = {{"header_enum", "A"}},
        .queries = {{"query_items", {"C", "D"}}},
        .body = R"(
{
   "first": "A",
   "second" : "D"
}
)"};
    auto test_req = parse_from_request<TestReq>(make_request_info(req));
    EXPECT_EQ(test_req.body.first, some_enum::A);
    EXPECT_EQ(test_req.body.second, some_enum::D);
    EXPECT_EQ(test_req.cookie_enum, some_enum::B);
    EXPECT_EQ(test_req.header_enum, some_enum::A);
    ASSERT_EQ(test_req.query_items.size(), 2);
    EXPECT_EQ(test_req.query_items[0], some_enum::C);
    EXPECT_EQ(test_req.query_items[1], some_enum::D);
}

