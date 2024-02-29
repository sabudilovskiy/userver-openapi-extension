#pragma once
#include <userver/formats/yaml.hpp>
#include <uopenapi/reflective/alias/alias_meta.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/reflective/reflectivable.hpp>
#include <uopenapi/pfr_extension/visit_struct.hpp>
#include <uopenapi/utils/is_optional/is_optional.hpp>

namespace uopenapi::reflective{

    struct schema{
        userver::formats::yaml::ValueBuilder v;
    };

    struct schema_view{
        userver::formats::yaml::ValueBuilder& root;
        userver::formats::yaml::ValueBuilder& cur_place;

        schema_view from_node(userver::formats::yaml::ValueBuilder& place){
            return schema_view{
                .root = root,
                .cur_place = place
            };
        }

        bool is_root(){
            return &root == &cur_place;
        }

        static schema_view from_schema(schema& schema){
            return{
                    .root = schema.v,
                    .cur_place = schema.v
            };
        }
    };
}