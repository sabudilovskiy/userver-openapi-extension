#include <mock_openapi_name.hpp>
#include <raw_string.hpp>
#include <uopenapi/all.hpp>
#include <userver/utest/utest.hpp>

namespace schema_enum_test {
enum struct SomeEnum { A, B, C, D };

struct SomeEnumInrospector {
    static constexpr std::array<std::string_view, 4> names{"A", "B", "C", "D"};
};

consteval SomeEnumInrospector get_enum_introspector(
    std::type_identity<SomeEnum>) {
    return {};
}
}  // namespace schema_enum_test

using schema_enum_test::SomeEnum;
using namespace uopenapi::reflective;

static_assert(uopenapi::has_introspector<SomeEnum>);

MOCK_OPENAPI_NAME(SomeEnum);

UTEST(openapi_schema_appenders, EnumBasic) {
    using appender = schema_appender<SomeEnum, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    SomeEnum:
      type: string
      enum:
        - A
        - B
        - C
        - D
)");
    auto str = ToString(s.v.ExtractValue());
    EXPECT_EQ(str, expected) << str;
}
