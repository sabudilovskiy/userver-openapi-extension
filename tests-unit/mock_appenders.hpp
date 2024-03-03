#pragma once

#define MOCK_APPENDER(TYPE, OPENAPI_TYPE_NAME) \
namespace uopenapi::reflective{ \
template <> \
struct schema_appender<TYPE, none_requirements>{ \
    template <none_requirements req> \
    static void append(schema_view view){ \
        auto& item_node = view.cur_place; \
        if (item_node.IsObject()){ \
            item_node = userver::formats::yaml::Type::kObject; \
        } \
        item_node["type"] = #OPENAPI_TYPE_NAME; \
    } \
}; \
}

struct MockString {};
struct MockInteger {};
struct MockOptionalString {};
struct MockOptionalInteger {};

namespace uopenapi::utils{
    template <>
    constexpr bool is_optional<MockOptionalString> = true;
    template <>
    constexpr bool is_optional<MockOptionalInteger> = true;
}

MOCK_APPENDER(MockString, string);
MOCK_APPENDER(MockOptionalString, string);
MOCK_APPENDER(MockInteger, integer);
MOCK_APPENDER(MockOptionalInteger, integer);