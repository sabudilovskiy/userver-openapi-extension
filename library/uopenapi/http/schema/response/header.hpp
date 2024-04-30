
#pragma once

#include <uopenapi/http/schema/response/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename T, typename Requirements>
requires reflective::has_schema_appender<utils::opt_raw_type_t<T>, Requirements>
struct schema_appender_response<T, Requirements, source_type::header> {
    using raw_type = utils::opt_raw_type_t<T>;
    using appender = reflective::schema_appender<raw_type, Requirements>;

    static void append(reflective::schema_view schemaView,
                       std::string_view field_name,
                       const Requirements& requirements) {
        auto& [root, cur] = schemaView;
        auto name_s = std::string{field_name};
        auto header = cur["headers"][name_s];
        auto header_schema = cur["headers"][name_s]["schema"];

        header["required"] = !utils::is_optional<T>;
        appender::append(schemaView.from_node(header_schema), requirements);
    }
};
}  // namespace uopenapi::http
