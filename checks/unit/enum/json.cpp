#include <uopenapi/enum/declare.hpp>
#include <uopenapi/json/string_like_parse.hpp>
#include <uopenapi/json/string_like_serialize.hpp>
#include <uopenapi/utils/converter/enum/all.hpp>
#include <userver/utest/utest.hpp>

namespace enum_tests {
UOPENAPI_DECLARE_ENUM(Color3, int, red, green, blue);
}  // namespace enum_tests
using enum_tests::Color3;
using enum enum_tests::Color3;

UTEST(EnumTests, JsonEnumSerialize) {
    userver::formats::json::ValueBuilder obj;
    obj["color1"] = red;
    obj["color2"] = green;
    obj["color3"] = blue;
    auto json = ToString(obj.ExtractValue());
    auto expected = R"({"color1":"red","color2":"green","color3":"blue"})";
    EXPECT_EQ(json, expected) << json;
}

UTEST(EnumTests, JsonEnumParse) {
    auto json_str = R"(
{
    "color1" : "red",
    "color2" : "green",
    "color3" : "blue"
}
)";
    auto obj = userver::formats::json::FromString(json_str);
    EXPECT_EQ(obj["color1"].As<Color3>(), red);
    EXPECT_EQ(obj["color2"].As<Color3>(), green);
    EXPECT_EQ(obj["color3"].As<Color3>(), blue);
}
