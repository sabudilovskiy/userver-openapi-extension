#pragma once
#include <uopenapi/components/schema/schema_storage.hpp>
#include <uopenapi/http/parse_request_info.hpp>
#include <uopenapi/http/schema_path.hpp>
#include <uopenapi/http/serialize_response_info.hpp>
#include <uopenapi/utils/constexpr_string.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <variant>

namespace uopenapi::components {
template <typename request, typename... responses>
struct openapi_handler : userver::server::handlers::HttpHandlerBase {
    using response = std::variant<responses...>;
    openapi_handler(const userver::components::ComponentConfig& cfg,
                    const userver::components::ComponentContext& ctx)
        : userver::server::handlers::HttpHandlerBase(cfg, ctx) {
        auto pStorage = ctx.FindComponentOptional<components::schema_storage>();
        if (!pStorage) {
            return;
        }
        auto& schema = pStorage->get_schema();
        append_to_schema(schema);
    }
    void append_to_schema(reflective::schema& schema) {
        auto& config = GetConfig();
        if (config.path.index() == 1) {
            return;
        }
        http::handler_info handlerInfo{.path = std::get<0>(config.path),
                                       .method = config.method};
        append_path<request, responses...>(schema, handlerInfo);
    }
    virtual response handle(request) const = 0;
    static http::response_info perform_r400(std::string_view msg) {
        userver::formats::json::ValueBuilder json;
        json["message"] = msg;
        return http::response_info{
            .body = ToString(json.ExtractValue()),
            .content_type = userver::http::content_type::kApplicationJson,
            .status_code = http::status_code_v<400>};
    }
    static http::response_info perform_r500(std::string_view msg) {
        userver::formats::json::ValueBuilder json;
        json["message"] = msg;
        return http::response_info{
            .body = ToString(json.ExtractValue()),
            .content_type = userver::http::content_type::kApplicationJson,
            .status_code = http::status_code_v<500>};
    }

    static std::optional<request> to_request(
        const userver::server::http::HttpRequest& httpReq,
        std::optional<http::response_info>& respInfo) {
        try {
            auto mapQuery = http::build_query_map(httpReq);
            // may throw, if throw must return 400
            return parse_from_request<request>(
                http::make_request_info(httpReq, mapQuery));
        } catch (std::exception& exc) {
            respInfo.emplace(perform_r400(fmt::format(
                "Some error happens where server tried to parse request: [{}]",
                exc.what())));
            return std::nullopt;
        }
    }

    static std::string write_response(
        const userver::server::http::HttpRequest& req,
        http::response_info& info) {
        auto& resp = req.GetHttpResponse();
        move_to_response(resp, info);
        resp.SetStatus(info.status_code);
        return std::move(info.body);
    }

   private:
    static http::response_info serialize_response(const response& resp) {
        try {
            return std::visit(
                [](auto& r) { return serialize_response_info(r); }, resp);
        } catch (std::exception& exc) {
            LOG_ERROR() << "Unexpected error from serialize: " << exc.what();
            return perform_r500("service unavailable");
        }
    }
    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& http_req,
        userver::server::request::RequestContext&) const override {
        std::optional<http::response_info> resp_info;
        auto req = to_request(http_req, resp_info);
        if (resp_info) {
            return write_response(http_req, *resp_info);
        }
        response resp = handle(*req);
        resp_info.emplace(serialize_response(resp));
        return write_response(http_req, resp_info.value());
    }
};

}  // namespace uopenapi::components
