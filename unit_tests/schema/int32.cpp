#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema_appenders, Int32None) {
    using appender = schema_appender<std::int32_t, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
format: int32
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, Int32Full) {
    constexpr auto req =
        uopenapi::reflective::number_requirements<std::int32_t>{
            .minimum = 1,
            .maximum = 6,
            .exclusive_minimum = true,
            .exclusive_maximum = true,
            .multiple_of = 3};
    using appender =
        schema_appender<std::int32_t, number_requirements<std::int32_t>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<req>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
format: int32
minimum: 1
exclusiveMinimum: true
maximum: 6
exclusiveMaximum: true
multipleOf: 3
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}
