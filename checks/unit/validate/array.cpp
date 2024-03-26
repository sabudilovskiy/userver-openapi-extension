#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_validates, ArrayMinItems) {
    std::vector<int> v;
    constexpr array_requirements req{.min_items = 1};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "array has size: [0] less than min_items: [1]");
}

UTEST(openapi_validates, ArrayMinItemsOk) {
    std::vector<int> v{1};
    constexpr array_requirements req{.min_items = 1};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validates, ArrayMaxItems) {
    std::vector<int> v{1, 2, 3, 4, 5};
    constexpr array_requirements req{.max_items = 4};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(validate_result.error_message(),
              "array has size: [5] greater than max_items: [4]");
}

UTEST(openapi_validates, ArrayMaxItemsOk) {
    std::vector<int> v{1, 2, 3, 4};
    constexpr array_requirements req{.max_items = 4};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}

UTEST(openapi_validates, ArrayUniqueItems) {
    std::vector<int> v{0, 2, 3, 4, 5, 0, 8};
    constexpr array_requirements req{.unique_items = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(validate_result.has_error());
    EXPECT_EQ(
        validate_result.error_message(),
        "array has non-unique items. first_index: [0], second_index: [5]");
}

UTEST(openapi_validates, ArrayUniqueItemsOk) {
    std::vector<int> v{0, 2, 3, 4, 5, 8};
    constexpr array_requirements req{.unique_items = true};
    auto validate_result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    EXPECT_TRUE(!validate_result.has_error());
}
