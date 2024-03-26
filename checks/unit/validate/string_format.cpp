#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

using namespace uopenapi::reflective;

UTEST(openapi_validates, StringFormatDatetime) {
    std::string v = "test";
    constexpr string_requirements<"date-time"> req{};
    auto result = validate(v, uopenapi::utils::create_nttp_adl<req>());
    ASSERT_TRUE(result.has_error());
    ASSERT_EQ(result.error_message(), "Can't parse datetime: test");
}
