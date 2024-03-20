#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema_appenders, StringNone){
    using appender = schema_appender<std::string, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, StringAllEmptyFormat){
    using appender = schema_appender<std::string, string_requirements<>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<string_requirements<>{
        .min_length = 1,
        .max_length = 5,
        .pattern = "[a-z]"
    }>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
minLength: 1
maxLength: 5
pattern: "[a-z]"
)");
    auto got = ToString(s.v.ExtractValue());
    EXPECT_EQ(got, expected);
}

UTEST(openapi_schema_appenders, StringAll){
    using appender = schema_appender<std::string, string_requirements<"date_time">>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<string_requirements<"date_time">{
            .min_length = 1,
            .max_length = 5,
            .pattern = "[a-z]"
    }>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
format: date_time
minLength: 1
maxLength: 5
pattern: "[a-z]"
)");
    auto got = ToString(s.v.ExtractValue());
    EXPECT_EQ(got, expected);
}