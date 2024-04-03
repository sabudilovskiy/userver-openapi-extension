#pragma once
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

#define MOCK_APPENDER(TYPE, OPENAPI_TYPE_NAME)                     \
    namespace uopenapi::reflective {                               \
    template <>                                                    \
    struct schema_appender<TYPE, none_requirements> {              \
        template <none_requirements req>                           \
        static void append(schema_view view) {                     \
            auto& item_node = view.cur_place;                      \
            if (item_node.IsObject()) {                            \
                item_node = userver::formats::yaml::Type::kObject; \
            }                                                      \
            item_node["type"] = #OPENAPI_TYPE_NAME;                \
        }                                                          \
    };                                                             \
    }

struct MockString {};
struct MockInteger {};
struct MockOptionalString {};
struct MockOptionalInteger {};

MOCK_APPENDER(MockString, string);
MOCK_APPENDER(MockOptionalString, string);
MOCK_APPENDER(MockInteger, integer);
MOCK_APPENDER(MockOptionalInteger, integer);

struct some_requirements {
    std::size_t max_length = 0;
};

namespace uopenapi::reflective {
template <>
struct schema_appender<MockString, some_requirements> {
    template <some_requirements req>
    static void append(schema_view view) {
        auto& item_node = view.cur_place;
        if (item_node.IsObject()) {
            item_node = userver::formats::yaml::Type::kObject;
        }
        item_node["type"] = "string";
        item_node["maxLength"] = req.max_length;
    }
};
}  // namespace uopenapi::reflective

namespace uopenapi::utils {
template <>
inline constexpr bool is_optional<MockOptionalString> = true;
template <>
inline constexpr bool is_optional<MockOptionalInteger> = true;

template <>
struct optional_getter<MockOptionalInteger> {
    using value_type = MockInteger;
};
template <>
struct optional_getter<MockOptionalString> {
    using value_type = MockString;
};
}  // namespace uopenapi::utils
