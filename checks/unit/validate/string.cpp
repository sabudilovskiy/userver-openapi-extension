#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_validates, StringMinLength){
    std::string v = "test";
    constexpr string_requirements<> req {
        .min_length = 5
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(), "str.size: [4] less than min_length: [5]");
}

UTEST(openapi_validates, StringMinLengthOk){
    std::string v = "test";
    constexpr string_requirements<> req {
            .min_length = 4
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validates, StringMaxLength){
    std::string v = "test";
    constexpr string_requirements<> req {
            .max_length = 3
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(), "str.size: [4] greater than max_length: [3]");
}

UTEST(openapi_validates, StringMaxLengthOk){
    std::string v = "test";
    constexpr string_requirements<> req {
            .max_length = 4
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validates, StringPattern){
    std::string v = "test";
    constexpr string_requirements<> req {
            .pattern = ".*r$"
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(), "str: [test], doesn't match with: [.*r$]");
}

UTEST(openapi_validates, StringPatternOk){
    std::string v = "test";
    constexpr string_requirements<> req {
            .pattern = ".*t$"
    };
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}
