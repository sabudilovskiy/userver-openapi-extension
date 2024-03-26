#pragma once

#include <string>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <userver/formats/json.hpp>

enum struct some_enum { A, B, C, D };

namespace uopenapi::utils {
template <>
struct converter<std::string, some_enum> {
    using source_type = std::string;
    using result_type = some_enum;
    static result_type convert(const source_type& source) {
        if (source == "A") {
            return some_enum::A;
        }
        if (source == "B") {
            return some_enum::B;
        }
        if (source == "C") {
            return some_enum::C;
        }
        if (source == "D") {
            return some_enum::D;
        }
        throw formatted_exception("unknown value: {}", source);
    }
};
template <>
struct converter<std::string_view, some_enum> {
    using source_type = std::string_view;
    using result_type = some_enum;
    static result_type convert(source_type source) {
        if (source == "A") {
            return some_enum::A;
        }
        if (source == "B") {
            return some_enum::B;
        }
        if (source == "C") {
            return some_enum::C;
        }
        if (source == "D") {
            return some_enum::D;
        }
        throw formatted_exception("unknown value: {}", source);
    }
};
template <>
struct converter<some_enum, std::string_view> {
    using source_type = some_enum;
    using result_type = std::string_view;
    static result_type convert(const source_type& source) {
        switch (source) {
            case some_enum::A:
                return "A";
            case some_enum::B:
                return "B";
            case some_enum::C:
                return "C";
            case some_enum::D:
                return "D";
        }
    }
};
template <>
struct converter<some_enum, std::string> {
    using source_type = some_enum;
    using result_type = std::string;
    static result_type convert(const source_type& source) {
        switch (source) {
            case some_enum::A:
                return "A";
            case some_enum::B:
                return "B";
            case some_enum::C:
                return "C";
            case some_enum::D:
                return "D";
        }
    }
};

}  // namespace uopenapi::utils

namespace uopenapi::reflective {
template <>
struct schema_appender<some_enum, none_requirements> {
    template <none_requirements>
    static void append(schema_view schemaView) {
        place_ref_to_type<some_enum>(schemaView.cur_place);
        auto type_node =
            schemaView
                .root["components"]["schemas"][schema_type_name<some_enum>()];
        type_node["type"] = "string";
        type_node["enum"] = userver::formats::yaml::Type::kArray;
        type_node["enum"].PushBack("A");
        type_node["enum"].PushBack("B");
        type_node["enum"].PushBack("C");
        type_node["enum"].PushBack("D");
    }
};
}  // namespace uopenapi::reflective

inline some_enum Parse(const userver::formats::json::Value& j,
                       userver::formats::parse::To<some_enum>) {
    std::string str = j.As<std::string>();
    return uopenapi::utils::converter<std::string, some_enum>::convert(str);
}

inline userver::formats::json::Value Serialize(
    const some_enum& v,
    userver::formats::serialize::To<userver::formats::json::Value>) {
    return userver::formats::json::ValueBuilder(
               uopenapi::utils::converter<some_enum, std::string_view>::convert(
                   v))
        .ExtractValue();
}
