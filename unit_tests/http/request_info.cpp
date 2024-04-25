#include <mock_converters.hpp>
#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::http;

namespace tests_request_http {
struct TestBody {
    some_enum first;
    some_enum second;
};
struct RequiredBody {
    TestBody body;
};
struct OptionalBody {
    std::optional<TestBody> body;
};
struct RequiredHeader {
    some_enum header_enum;
};
struct OptionalHeader {
    std::optional<some_enum> header_enum;
};
struct RequiredCookie {
    some_enum cookie_enum;
};
struct OptionalCookie {
    std::optional<some_enum> cookie_enum;
};
struct RequiredQuery {
    some_enum query_enum;
};
struct OptionalQuery {
    std::optional<some_enum> query_enum;
};
struct ArrayQuery {
    std::vector<some_enum> query_enum;
};
}  // namespace tests_request_http

using namespace tests_request_http;
UOPENAPI_SOURCE_TYPE(RequiredHeader, header_enum, header);
UOPENAPI_SOURCE_TYPE(OptionalHeader, header_enum, header);
UOPENAPI_SOURCE_TYPE(RequiredCookie, cookie_enum, cookie);
UOPENAPI_SOURCE_TYPE(OptionalCookie, cookie_enum, cookie);

UTEST(http_request_parse, OptionalBodyMissing) {
    test_request req{.body = ""};
    auto test_req = parse_from_request<OptionalBody>(make_request_info(req));
    EXPECT_EQ(test_req.body, std::nullopt);
}

UTEST(http_request_parse, OptionalBodyExist) {
    test_request req{
        .body = R"(
{
   "first": "A",
   "second" : "D"
}
)"};
    auto test_req = parse_from_request<OptionalBody>(make_request_info(req));
    ASSERT_EQ(test_req.body.has_value(), true);
    EXPECT_EQ(test_req.body->first, some_enum::A);
    EXPECT_EQ(test_req.body->second, some_enum::D);
}

UTEST(http_request_parse, RequiredBodyMissing) {
    test_request req{.body = ""};
    auto test = [&](){parse_from_request<RequiredBody>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}

UTEST(http_request_parse, RequiredBodyExist) {
    test_request req{
        .body = R"(
{
   "first": "A",
   "second" : "D"
}
)"};
    auto test_req = parse_from_request<RequiredBody>(make_request_info(req));
    EXPECT_EQ(test_req.body.first, some_enum::A);
    EXPECT_EQ(test_req.body.second, some_enum::D);
}

UTEST(http_request_parse, RequiredHeaderMissing) {
    test_request req{};
    auto test = [&](){parse_from_request<RequiredHeader>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}

UTEST(http_request_parse, RequiredHeaderExist) {
    test_request req{
        .headers = {{"header_enum", "A"}}
    };
    auto value = parse_from_request<RequiredHeader>(make_request_info(req));
    EXPECT_EQ(value.header_enum, some_enum::A);
}

UTEST(http_request_parse, OptionalHeaderMissing) {
    test_request req{
    };
    auto value = parse_from_request<OptionalHeader>(make_request_info(req));
    EXPECT_EQ(value.header_enum, std::nullopt);
}

UTEST(http_request_parse, OptionalHeaderExist) {
    test_request req{
        .headers = {{"header_enum", "A"}}
    };
    auto value = parse_from_request<OptionalHeader>(make_request_info(req));
    EXPECT_EQ(value.header_enum, some_enum::A);
}

UTEST(http_request_parse, RequiredCookieMissing) {
    test_request req{};
    auto test = [&](){parse_from_request<RequiredCookie>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}

UTEST(http_request_parse, RequiredCookieExist) {
    test_request req{
        .cookies = {{"cookie_enum", "A"}}
    };
    auto value = parse_from_request<RequiredCookie>(make_request_info(req));
    EXPECT_EQ(value.cookie_enum, some_enum::A);
}

UTEST(http_request_parse, OptionalCookieMissing) {
    test_request req{
    };
    auto value = parse_from_request<OptionalCookie>(make_request_info(req));
    EXPECT_EQ(value.cookie_enum, std::nullopt);
}

UTEST(http_request_parse, OptionalCookieExist) {
    test_request req{
        .cookies = {{"cookie_enum", "A"}}
    };
    auto value = parse_from_request<OptionalCookie>(make_request_info(req));
    EXPECT_EQ(value.cookie_enum, some_enum::A);
}

UTEST(http_request_parse, RequiredQueryMissing) {
    test_request req{};
    auto test = [&](){parse_from_request<RequiredQuery>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}

UTEST(http_request_parse, RequiredQueryExist) {
    test_request req{
        .queries = {{"query_enum",{"A"}}}
    };
    auto value = parse_from_request<RequiredQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum, some_enum::A);
}

UTEST(http_request_parse, RequiredQueryMore) {
    test_request req{
        .queries = {{"query_enum",{"A", "B"}}}
    };
    auto test = [&](){parse_from_request<RequiredQuery>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}

UTEST(http_request_parse, OptionalQueryMissing) {
    test_request req{
    };
    auto value = parse_from_request<OptionalQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum, std::nullopt);
}

UTEST(http_request_parse, OptionalQueryExist) {
    test_request req{
        .queries = {{"query_enum",{"A"}}}
    };
    auto value = parse_from_request<OptionalQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum, some_enum::A);
}

UTEST(http_request_parse, OptionalQueryMore) {
    test_request req{
        .queries = {{"query_enum",{"A", "B"}}}
    };
    auto test = [&](){parse_from_request<OptionalQuery>(make_request_info(req));};
    EXPECT_THROW(test(), std::runtime_error);
}


UTEST(http_request_parse, ArrayQueryMissing) {
    test_request req{
    };
    auto value = parse_from_request<ArrayQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum.size(), 0);
}

UTEST(http_request_parse, ArrayQueryExist) {
    test_request req{
        .queries = {{"query_enum",{"A"}}}
    };
    auto value = parse_from_request<ArrayQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum.size(), 1);
    EXPECT_EQ(value.query_enum[0], some_enum::A);
}

UTEST(http_request_parse, ArrayQueryMore) {
    test_request req{
        .queries = {{"query_enum",{"A", "B"}}}
    };
    auto value = parse_from_request<ArrayQuery>(make_request_info(req));
    EXPECT_EQ(value.query_enum.size(), 2);
    EXPECT_EQ(value.query_enum[0], some_enum::A);
    EXPECT_EQ(value.query_enum[1], some_enum::B);
}
