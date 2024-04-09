#pragma once
#include <uopenapi/components/schema/schema_storage.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace uopenapi::components {
struct schema_http_distributor : userver::server::handlers::HttpHandlerBase {
    static constexpr std::string_view kName = "schema-http-distributor";
    schema_http_distributor(const userver::components::ComponentConfig& cfg,
                            const userver::components::ComponentContext& ctx)
        : HttpHandlerBase(cfg, ctx),
          storage(ctx.FindComponent<schema_storage>()) {}
    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& req,
        userver::server::request::RequestContext&) const override {
        req.SetResponseStatus(userver::server::http::HttpStatus::kOk);
        req.GetHttpResponse().SetContentType(
            userver::http::content_type::kTextPlain);
        return schema_str;
    }
    void OnAllComponentsLoaded() override {
        auto schema = storage.get_schema().v;
        schema_str = ToString(schema.ExtractValue());
    }

   private:
    std::string schema_str;
    const schema_storage& storage;
};
}  // namespace uopenapi::components
