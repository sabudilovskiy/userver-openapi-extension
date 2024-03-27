#include <uopenapi/enum/declare.hpp>
#include <userver/utest/utest.hpp>

namespace enum_tests {
    UOPENAPI_DECLARE_ENUM(Color, int, red, green, blue);

    UOPENAPI_DECLARE_ENUM(AnotherColor, int, red = 1, green = 5, blue = 7);
}
using enum_tests::Color;
using enum_tests::AnotherColor;
static_assert(uopenapi::has_introspector<Color>);
static_assert(uopenapi::has_introspector<AnotherColor>);

UTEST(EnumTests, BasicDeclare) {
    using introspector = uopenapi::enum_introspector<Color>;
    auto names = introspector::names;
    auto values= introspector ::values;
    ASSERT_EQ(names.size(), 3);
    ASSERT_EQ(values.size(), 3);
    ASSERT_EQ(names[0], "red");
    ASSERT_EQ(names[1], "green");
    ASSERT_EQ(names[2], "blue");
    ASSERT_EQ(values[0], Color::red);
    ASSERT_EQ(values[1], Color::green);
    ASSERT_EQ(values[2], Color::blue);
}

UTEST(EnumTests, BasicDeclareWithValues) {
    using introspector = uopenapi::enum_introspector<AnotherColor>;
    auto names = introspector::names;
    auto values= introspector ::values;
    ASSERT_EQ(names.size(), 3);
    ASSERT_EQ(values.size(), 3);
    ASSERT_EQ(names[0], "red");
    ASSERT_EQ(names[1], "green");
    ASSERT_EQ(names[2], "blue");
    ASSERT_EQ(values[0], AnotherColor::red);
    ASSERT_EQ(values[1], AnotherColor::green);
    ASSERT_EQ(values[2], AnotherColor::blue);
}
