#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema, StringNone) {
    using appender = schema_appender<std::string, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append(view, none_requirements{});
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema, StringAllEmptyFormat) {
    using appender = schema_appender<std::string, string_requirements<>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append(view,
                     string_requirements<>{
                         .min_length = 1, .max_length = 5, .pattern = "[a-z]"});
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
minLength: 1
maxLength: 5
pattern: "[a-z]"
)");
    auto got = ToString(s.v.ExtractValue());
    EXPECT_EQ(got, expected);
}

UTEST(openapi_schema, StringAll) {
    using appender =
        schema_appender<std::string, string_requirements<"date_time">>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append(view,
                     string_requirements<"date_time">{
                         .min_length = 1, .max_length = 5, .pattern = "[a-z]"});
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

UTEST(openapi_schema, Uuid) {
    using appender = schema_appender<boost::uuids::uuid, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append(view, none_requirements{});
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
format: uuid
)");
    auto got = ToString(s.v.ExtractValue());
    EXPECT_EQ(got, expected);
}

UTEST(openapi_schema, UserverDate) {
    using appender =
        schema_appender<userver::utils::datetime::Date, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append(view, none_requirements{});
    auto expected = UOPENAPI_RAW_STRING(R"(
type: string
format: date
)");
    auto got = ToString(s.v.ExtractValue());
    EXPECT_EQ(got, expected);
}
