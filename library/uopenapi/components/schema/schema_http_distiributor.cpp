#include <userver/components/component_context.hpp>

#include "schema_http_distributor.hpp"

namespace uopenapi::components {
schema_http_distributor::schema_http_distributor(
    const userver::components::ComponentConfig& cfg,
    const userver::components::ComponentContext& ctx)
    : HttpHandlerBase(cfg, ctx), storage(ctx.FindComponent<schema_storage>()) {}

std::string schema_http_distributor::HandleRequestThrow(
    const userver::server::http::HttpRequest& req,
    userver::server::request::RequestContext&) const {
    req.SetResponseStatus(userver::server::http::HttpStatus::kOk);
    req.GetHttpResponse().SetContentType(
        userver::http::content_type::kTextPlain);
    return schema_str;
}

void schema_http_distributor::OnAllComponentsLoaded() {
    auto schema = storage.get_schema().v;
    schema_str = ToString(schema.ExtractValue());
}

}  // namespace uopenapi::components
