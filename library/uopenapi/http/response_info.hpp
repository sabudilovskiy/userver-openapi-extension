#pragma once
#include <unordered_map>
#include <userver/server/http/http_request.hpp>

namespace uopenapi::http {

struct response_info {
    using headers_map = std::unordered_map<std::string_view, std::string>;
    using cookies_map = headers_map;

    headers_map headers;
    cookies_map cookies;
    std::string body;
    const userver::http::ContentType& content_type;
    userver::server::http::HttpStatus status_code =
        userver::server::http::HttpStatus::kOk;
};

inline void move_to_response(userver::server::http::HttpResponse& res,
                             response_info& info) {
    for (auto&& [h, value] : info.headers) {
        res.SetHeader(h, std::move(value));
    }
    for (auto&& [c, value] : info.cookies) {
        userver::server::http::Cookie cookie(std::string{c}, std::move(value));
        res.SetCookie(std::move(cookie));
    }
}

}  // namespace uopenapi::http