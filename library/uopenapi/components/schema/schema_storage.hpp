#pragma once

#include <string_view>
#include <uopenapi/reflective/schema/schema.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/formats/yaml/serialize.hpp>
#include <userver/http/content_type.hpp>

namespace uopenapi::components {
struct schema_storage : public userver::components::LoggableComponentBase {
    static constexpr std::string_view kName = "schema-storage";
    schema_storage(const userver::components::ComponentConfig& cfg,
                   const userver::components::ComponentContext& ctx)
        : userver::components::LoggableComponentBase(cfg, ctx) {
        schema.v["info"] = cfg["info"].Yaml();
        schema.v["openapi"] = "3.0.0";
        schema.v["servers"] = cfg["servers"].Yaml();
    }

    reflective::schema& get_schema() { return schema; }
    const reflective::schema& get_schema() const { return schema; }

    static userver::yaml_config::Schema GetStaticConfigSchema();

   private:
    reflective::schema schema;
};
}  // namespace uopenapi::components
