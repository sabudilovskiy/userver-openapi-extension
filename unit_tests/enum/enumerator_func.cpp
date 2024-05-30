#include <uopenapi/enum_helpers/declare.hpp>
#include <uopenapi/enum_helpers/postgres/enumerator_func.hpp>
#include <userver/utest/utest.hpp>

namespace enum_tests {
UOPENAPI_DECLARE_ENUM(Color2, int, red, green, blue);
}  // namespace enum_tests
using enum_tests::Color2;

UTEST(enum_tests, BasicEnumeratorFunc) {
    using enum enum_tests::Color2;
    userver::utils::TrivialBiMap biMap =
        uopenapi::enum_helpers::create_enumerator_func<Color2>();
    EXPECT_EQ(biMap.TryFind(red), "red");
    EXPECT_EQ(biMap.TryFind(blue), "blue");
    EXPECT_EQ(biMap.TryFind(green), "green");
    EXPECT_EQ(biMap.TryFindICase("red"), red);
    EXPECT_EQ(biMap.TryFindICase("blue"), blue);
    EXPECT_EQ(biMap.TryFindICase("green"), green);
}
