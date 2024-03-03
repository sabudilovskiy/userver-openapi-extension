#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_schema_appenders, GenericInt16None){
    using appender = schema_appender<std::int16_t , none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
minimum: -32768
maximum: 32767
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, GenericInt16Full){
    constexpr auto req = uopenapi::reflective::number_requirements<std::int16_t>{
            .minimum = 1,
            .maximum = 6,
            .exclusive_minimum = true,
            .exclusive_maximum = true,
            .multiple_of = 3
    };
    using appender = schema_appender<std::int16_t, number_requirements<std::int16_t>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<req>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
minimum: 1
maximum: 6
exclusiveMinimum: true
exclusiveMaximum: true
multipleOf: 3
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, GenericInt16Part){
    constexpr auto req = uopenapi::reflective::number_requirements<std::int16_t>{
            .exclusive_minimum = true,
            .exclusive_maximum = true,
            .multiple_of = 3
    };
    using appender = schema_appender<std::int16_t, number_requirements<std::int16_t>>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<req>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: integer
minimum: -32768
maximum: 32767
exclusiveMinimum: true
exclusiveMaximum: true
multipleOf: 3
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}