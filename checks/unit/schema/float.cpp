#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema_appenders, FloatNone) {
    using appender = schema_appender<float, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: number
format: float
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, FloatReqs) {
    constexpr auto req = uopenapi::reflective::number_requirements<float>{
        .minimum = 1.5,
        .maximum = 6.5,
        .exclusive_minimum = true,
        .exclusive_maximum = true,
        .multiple_of = 2};
    using appender = schema_appender<float, number_requirements<float>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<req>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: number
format: float
minimum: 1.5
exclusiveMinimum: true
maximum: 6.5
exclusiveMaximum: true
multipleOf: 2
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}
