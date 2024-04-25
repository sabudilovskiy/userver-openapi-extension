#pragma once
#include <string_view>

namespace uopenapi::http {
enum struct source_type { body_JSON, query, header, cookie };

constexpr std::string_view to_string_view(source_type sourceType) {
    switch (sourceType) {
        case source_type::body_JSON:
            return "body_JSON";
            ;
        case source_type::query:
            return "query";
            ;
        case source_type::header:
            return "header";
            ;
        case source_type::cookie:
            return "cookie";
            ;
    }
}
}  // namespace uopenapi::http
