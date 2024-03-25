#pragma once
#include <userver/server/http/http_request.hpp>

namespace uopenapi::http {

struct request_info {
    using cookies_map = userver::server::http::HttpRequest::CookiesMap;
    using headers_map = userver::server::http::HttpRequest::HeadersMap;
    using queries_map =
        std::unordered_map<std::string, std::vector<std::string>>;

    const cookies_map& cookies;
    const headers_map& headers;
    const queries_map& queries;

    std::string_view body;
};
struct test_request {
    userver::server::http::HttpRequest::CookiesMap cookies;
    userver::server::http::HttpRequest::HeadersMap headers;
    request_info::queries_map queries;

    std::string body;
};

inline request_info::queries_map build_query_map(
    const userver::server::http::HttpRequest& request) {
    auto names = request.ArgNames();
    request_info::queries_map queriesMap;
    for (auto& name : names) {
        queriesMap[name] = request.GetArgVector(name);
    }
    return queriesMap;
}

inline request_info make_request_info(
    const userver::server::http::HttpRequest& request,
    const request_info::queries_map& queries) {
    return request_info{.cookies = request.RequestCookies(),
                        .headers = request.GetHeaders(),
                        .queries = queries,
                        .body = request.RequestBody()};
}
inline request_info make_request_info(const test_request& request) {
    return request_info{.cookies = request.cookies,
                        .headers = request.headers,
                        .queries = request.queries,
                        .body = request.body};
}

}  // namespace uopenapi::http
