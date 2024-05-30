#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_validate, DoubleMinimum) {
    double v = 1.5;
    constexpr number_requirements<double> req{.minimum = 2};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "value: [1.5] less than minumum: [2]");
}

UTEST(openapi_validate, DoubleMinimumOk) {
    double v = 1.5;
    constexpr number_requirements<double> req{.minimum = 1.0};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validate, DoubleMaximum) {
    double v = 10.5;
    constexpr number_requirements<double> req{.maximum = 2};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "value: [10.5] greater than maximum: [2]");
}

UTEST(openapi_validate, DoubleMaximumOk) {
    double v = 10.5;
    constexpr number_requirements<double> req{.maximum = 11};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validate, DoubleMinExclusive) {
    double v = 1;
    constexpr number_requirements<double> req{.minimum = 1,
                                              .exclusive_minimum = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "value: [1] must be greater than exclusive minimum: [1]");
}

UTEST(openapi_validate, DoubleMinExclusiveOk) {
    double v = 1.000001;
    constexpr number_requirements<double> req{.minimum = 1,
                                              .exclusive_minimum = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validate, DoubleMaxExclusive) {
    double v = 1;
    constexpr number_requirements<double> req{.maximum = 1,
                                              .exclusive_maximum = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "value: [1] must be less than exclusive maximum: [1]");
}

UTEST(openapi_validate, DoubleMaxExclusiveOk) {
    double v = 0.9999999;
    constexpr number_requirements<double> req{.maximum = 1,
                                              .exclusive_maximum = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validate, DoudbleMultiplyOf) {
    double v = 2.6;
    constexpr number_requirements<double> req{.multiple_of = 0.5};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "value: [2.6] must be multiplyOf: [0.5]");
}

UTEST(openapi_validate, DoudbleMultiplyOfOk) {
    double v = 2.5;
    constexpr number_requirements<double> req{.multiple_of = 0.5};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}
