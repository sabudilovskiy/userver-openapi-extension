#pragma once
#include <uopenapi/http/source_type.hpp>

namespace uopenapi::http {
struct request_info;

template <typename T, source_type sourceType>
struct request_parser {
    static T parse(const http::request_info&, std::string_view) {
        static_assert(sizeof(T) == 0, "Fallback parse. Write a specialization");
    }
};

}  // namespace uopenapi::http
