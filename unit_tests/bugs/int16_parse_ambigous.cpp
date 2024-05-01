#include <cstdint>
#include <uopenapi/all.hpp>
#include <userver/formats/parse/common.hpp>
#include <userver/utest/utest.hpp>

UTEST(Bugs, Int16tParseAmibigous) {
    userver::formats::json::ValueBuilder test =
        userver::formats::json::Type::kObject;
    test["ss"] = 111;
    auto value = test.ExtractValue();
    auto val = value["ss"].As<std::int16_t>();
    EXPECT_EQ(val, 111);
}
