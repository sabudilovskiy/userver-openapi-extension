#include <mock_appenders.hpp>
#include <mock_converters.hpp>
#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::http;

namespace tests_response_info {
struct Body {
    some_enum first;
    some_enum second;
};
struct RequiredBody {
    Body body;
};
struct OptionalBody {
    std::optional<Body> body;
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

struct CORS{};

struct RespWithCors{
    CORS cors;
    Body body;
};
}  // namespace tests_response_info
using namespace tests_response_info;
UOPENAPI_SOURCE_TYPE(RespWithCors, cors, header);
UOPENAPI_SOURCE_TYPE(RequiredHeader, header_enum, header);
UOPENAPI_SOURCE_TYPE(OptionalHeader, header_enum, header);
UOPENAPI_SOURCE_TYPE(RequiredCookie, cookie_enum, cookie);
UOPENAPI_SOURCE_TYPE(OptionalCookie, cookie_enum, cookie);


namespace uopenapi::http{
    template <>
    struct response_serializator<CORS, source_type::header>{
        static void serialize(const CORS&, response_info& r_info, std::string_view){
            auto add_header = [&](std::string_view name_header, std::string_view value){
                auto [it, emplaced] = r_info.headers.emplace(name_header, value);
                if (!emplaced){
                    throw utils::formatted_exception("Detected multiply value of header [{}], first value: [{}], second value: [{}]", name_header, it->second, value);
                }
            };
            add_header("Access-Control-Allow-Methods", "GET, POST");
            add_header("Access-Control-Allow-Origin", "*");
            add_header("Access-Control-Allow-Credentials", "true");
            add_header("Access-Control-Allow-Headers", "GET, POST");
            add_header("Access-Control-Expose-Headers", "*");
        }
    };
}

UTEST(http_response_serialize, RequiredBodyExist) {
    RequiredBody resp{
        .body = Body{.first = some_enum::A, .second = some_enum::B},
    };
    auto resp_info = serialize_response_info(resp);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kApplicationJson);
    EXPECT_EQ(resp_info.body, R"({"first":"A","second":"B"})");
}

UTEST(http_response_serialize, OptionalBodyExist) {
    OptionalBody resp{
        .body = Body{.first = some_enum::A, .second = some_enum::B},
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kApplicationJson);
    EXPECT_EQ(resp_info.body, R"({"first":"A","second":"B"})");
}

UTEST(http_response_serialize, RequiredHeaderExist) {
    RequiredHeader resp{
        .header_enum = some_enum::C
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.headers, (RespInfo::headers_map{{"header_enum", "C"}}));
}

UTEST(http_response_serialize, OptionalHeaderExist) {
    OptionalHeader resp{
        .header_enum = some_enum::C
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.headers, (RespInfo::headers_map{{"header_enum", "C"}}));
}

UTEST(http_response_serialize, OptionalHeaderMissing) {
    OptionalHeader resp{
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.headers, (RespInfo::headers_map{}));
}

UTEST(http_response_serialize, RequiredCookieExist) {
    RequiredCookie resp{
        .cookie_enum = some_enum::C
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.cookies, (RespInfo::cookies_map{{"cookie_enum", "C"}}));
}

UTEST(http_response_serialize, OptionalCookieExist) {
    OptionalCookie resp{
        .cookie_enum = some_enum::C
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.cookies, (RespInfo::cookies_map{{"cookie_enum", "C"}}));
}

UTEST(http_response_serialize, OptionalCookieMissing) {
    OptionalCookie resp{
    };
    auto resp_info = serialize_response_info(resp);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kTextPlain);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.cookies, (RespInfo::cookies_map{}));
}

UTEST(http_response_serialize, AddCORS) {
    //    uopenapi::http::response_serializator<some_enum,
    //    uopenapi::http::source_type::header>::serialize();
    RespWithCors resp{
        .body = Body{.first = some_enum::A, .second = some_enum::B},
    };
    auto resp_info = serialize_response_info(resp);
    using RespInfo = decltype(resp_info);
    EXPECT_EQ(resp_info.content_type,
              userver::http::content_type::kApplicationJson);
    EXPECT_EQ(resp_info.body, R"({"first":"A","second":"B"})");
    EXPECT_EQ(resp_info.headers, (RespInfo::headers_map{
                                     {"Access-Control-Allow-Methods", "GET, POST"},
                                     {"Access-Control-Allow-Origin", "*"},
                                     {"Access-Control-Allow-Credentials", "true"},
                                     {"Access-Control-Allow-Headers", "GET, POST"},
                                     {"Access-Control-Expose-Headers", "*"},
                                 }));
}
