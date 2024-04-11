#pragma once
#include <uopenapi/components/schema/schema_storage.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace uopenapi::components {
struct schema_http_distributor : userver::server::handlers::HttpHandlerBase {
    static constexpr std::string_view kName = "schema-http-distributor";
    schema_http_distributor(const userver::components::ComponentConfig& cfg,
                            const userver::components::ComponentContext& ctx);
    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& req,
        userver::server::request::RequestContext&) const override;
    void OnAllComponentsLoaded() override;

   private:
    std::string schema_str;
    const schema_storage& storage;
};
}  // namespace uopenapi::components
