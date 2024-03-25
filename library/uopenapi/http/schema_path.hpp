#pragma once
#include <uopenapi/http/schema_request.h>
#include <uopenapi/http/schema_response.h>

#include <string>
#include <uopenapi/http/response.hpp>

namespace uopenapi::http {
struct handler_info {
    const std::string& path;
    const std::string& method;
};

namespace details {
template <typename T, std::size_t Code>
void call_append_response(
    reflective::schema_view schemaView,
    std::type_identity<uopenapi::http::response<T, Code>>) {
    append_response<T, Code>(schemaView);
}

inline std::vector<std::string> split_methods(handler_info handlerInfo) {
    std::string method_low;
    std::vector<std::string> methods;
    for (auto ch : handlerInfo.method) {
        if (ch != ',') {
            method_low.append(1, tolower(ch));
        } else {
            methods.emplace_back(std::move(method_low));
        }
    }
    if (!method_low.empty()) {
        methods.emplace_back(std::move(method_low));
    }
    return methods;
}
}  // namespace details

template <typename request, typename... responses>
void append_path(reflective::schema& schema, handler_info handlerInfo) {
    auto schemaView = reflective::schema_view::from_schema(schema);
    auto& [path, method_raw] = handlerInfo;
    auto methods = details::split_methods(handlerInfo);
    for (auto& method : methods) {
        auto method_node = schemaView.root["paths"][path][method];
        details::append_request<request>(schemaView.from_node(method_node));
        auto responses_node = method_node["responses"];
        auto responses_view = schemaView.from_node(responses_node);
        (details::call_append_response(responses_view,
                                       std::type_identity<responses>{}),
         ...);
    }
}
}  // namespace uopenapi::http