#pragma once

#include <string_view>
#include <uopenapi/http/source_type.hpp>

namespace uopenapi::http {
struct request_info;

template <typename T, source_type SourceType>
struct request_parser {
    // static T parse(const http::request_info&, std::string_view) {
    //     static_assert(sizeof(T) == 0, "Fallback parse. Write a
    //     specialization");
    // }
};

template <typename T, source_type SourceType>
concept has_request_parser =
    requires(const request_info& reqInfo, std::string_view name) {
        request_parser<T, SourceType>::parse(reqInfo, name);
    };

}  // namespace uopenapi::http
