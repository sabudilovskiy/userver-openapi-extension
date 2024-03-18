#include <userver/utest/utest.hpp>
#include <uopenapi/all.hpp>
#include <raw_string.hpp>
#include <mock_appenders.hpp>
#include <mock_openapi_name.hpp>

using namespace uopenapi::reflective;

inline namespace schema_tests_object{

    struct BasicObject {
        MockString f1;
        MockInteger f2;
    };

    struct BasicObjectOpt {
        MockString f1;
        MockInteger f2;
        MockOptionalString f3;
        MockOptionalInteger f4;
    };

}

MOCK_OPENAPI_NAME(BasicObject);
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
      properties:
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
      properties:
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

struct BasicObjectReqFields{
    MockString f1;
    MockString f2;
};

MOCK_OPENAPI_NAME(BasicObjectReqFields);

template <> constexpr auto ::uopenapi::reflective::requirements_field< BasicObjectReqFields, "f1"> = some_requirements{
    .max_length = 1
};

template <> constexpr auto ::uopenapi::reflective::requirements_field< BasicObjectReqFields, "f2"> = some_requirements{
    .max_length = 5
};

UTEST(openapi_schema_appenders, ObjectReqFields){
    using appender = schema_appender<BasicObjectReqFields, none_requirements>;
    schema s;
    auto view = schema_view::from_schema(s);
    appender::append<none_requirements{}>(view);
    auto expected = UOPENAPI_RAW_STRING(R"(
components:
  schemas:
    BasicObjectReqFields:
      type: object
      properties:
        f1:
          type: string
          maxLength: 1
        f2:
          type: string
          maxLength: 5
      required:
        - f1
        - f2
)");
    EXPECT_EQ(ToString(s.v.ExtractValue()), expected);
}