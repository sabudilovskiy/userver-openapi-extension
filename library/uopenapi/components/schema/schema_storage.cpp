#include "schema_storage.hpp"

#include <userver/yaml_config/merge_schemas.hpp>

namespace uopenapi::components {
userver::yaml_config::Schema schema_storage::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<
        userver::components::LoggableComponentBase>(
        R"(
type: object
description: Class describe by openapi server
additionalProperties: true
properties:
  servers:
    description: servers section in openapi
    type: array
    items:
      description: one server
      type: object
      properties: {}
      additionalProperties: true
  info:
    type: object
    description: info section in openapi
    properties:
      title:
        type: string
        description: info_titile
      description:
        type: string
        description: info_description
      version:
        type: string
        description: info_version
    additionalProperties: true
)");
}
}  // namespace uopenapi::components
