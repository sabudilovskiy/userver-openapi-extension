#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>

using namespace uopenapi::reflective;

namespace {

    struct MockType {

    };

}

namespace uopenapi::reflective{
    template <>
    struct schema_appender<MockType, none_requirements>{
        template <none_requirements req>
        static void append(schema_view view){
            auto& item_node = view.cur_place;
            if (item_node.IsObject()){
                item_node = userver::formats::yaml::Type::kObject;
            }
            item_node["type"] = "string";
        }
    };
}

UTEST(openapi_schema_appenders, ArrayNone){
    using appender = schema_appender<std::vector<MockType>, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: array
items:
  type: string
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, ArrayFull){
    using appender = schema_appender<std::vector<MockType>, array_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<array_requirements{
        .min_items = 1,
        .max_items = 5,
        .unique_items = true
    }>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
type: array
minItems: 1
maxItems: 5
uniqueItems: true
items:
  type: string
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}
