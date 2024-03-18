#pragma once

#include <string_view>
#include <userver/components/component_config.hpp>
#include <userver/formats/yaml/serialize.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <uopenapi/reflective/schema/schema.hpp>

namespace uopenapi::http
{
    struct openapi_descriptor : public userver::server::handlers::HttpHandlerBase
    {
        static constexpr std::string_view kName = "openapi-descriptor";
        openapi_descriptor(const userver::components::ComponentConfig& cfg,
                          const userver::components::ComponentContext& ctx)
                : userver::server::handlers::HttpHandlerBase(cfg, ctx)
        {
            schema.v["info"] = cfg["info"].Yaml();
            schema.v["openapi"] = "3.0.0";
            schema.v["servers"] = cfg["servers"].Yaml();
        }
        std::string HandleRequestThrow(
                const userver::server::http::HttpRequest& req,
                userver::server::request::RequestContext&) const override
        {
            req.SetResponseStatus(userver::server::http::HttpStatus::kOk);
            req.GetHttpResponse().SetContentType(
                    userver::http::content_type::kTextPlain);
            return schema_str;
        }

        void OnAllComponentsLoaded() override
        {
            schema_str = ToString(schema.v.ExtractValue());
        }
        reflective::schema& get_schema()
        {
            return schema;
        }

        static userver::yaml_config::Schema GetStaticConfigSchema();

    private:
        reflective::schema schema;
        std::string schema_str;
    };
}  // namespace openapi::http