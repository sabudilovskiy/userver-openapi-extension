#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema_appenders, Int64None){
    using appender = schema_appender<std::int64_t, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
format: int64
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, Int64Full){
    constexpr auto req = uopenapi::reflective::number_requirements<std::int64_t>{
            .minimum = 1,
            .maximum = 6,
            .exclusive_minimum = true,
            .exclusive_maximum = true,
            .multiple_of = 3
    };
    using appender = schema_appender<std::int64_t, number_requirements<std::int64_t>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<req>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
format: int64
minimum: 1
exclusiveMinimum: true
maximum: 6
exclusiveMaximum: true
multipleOf: 3
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}
