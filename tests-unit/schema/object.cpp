#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>
#include <mock_appenders.hpp>
#include <mock_openapi_name.hpp>

using namespace uopenapi::reflective;

struct BasicObject{
    MockString f1;
    MockInteger f2;
};

MOCK_OPENAPI_NAME(BasicObject);

struct BasicObjectOpt{
    MockString f1;
    MockInteger f2;
    MockOptionalString f3;
    MockOptionalInteger f4;
};

MOCK_OPENAPI_NAME(BasicObjectOpt);


UTEST(openapi_schema_appenders, BasicObject){
    using appender = schema_appender<BasicObject, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    BasicObject:
      type: object
      fields:
        f1:
          type: string
        f2:
          type: integer
      required:
        - f1
        - f2
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}

UTEST(openapi_schema_appenders, ObjectOptionalFields){
    using appender = schema_appender<BasicObjectOpt, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    BasicObjectOpt:
      type: object
      fields:
        f1:
          type: string
        f2:
          type: integer
        f3:
          type: string
        f4:
          type: integer
      required:
        - f1
        - f2
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}