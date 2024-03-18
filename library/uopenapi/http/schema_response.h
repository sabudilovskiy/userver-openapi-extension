#pragma once

#include <uopenapi/reflective/schema/appender.hpp>
#include <uopenapi/http/source_type.hpp>
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/reflective/schema/schema.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
    namespace details {
        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> == source_type::header)
        void append_response_field(reflective::schema_view schemaView) {
            using F = pfr_extension::tuple_element_name_t<T, name>;
            auto& [root, cur] = schemaView;
            auto name_s = name.AsString();
            auto header = cur["headers"][name_s]["schema"];

            if constexpr (!utils::is_optional<F>){
                reflective::call_append<T, name, F>(schemaView.from_node(header));
                header["required"] = true;
            }
            else {
                using raw_type = utils::optional_getter_t<T>;
                reflective::call_append<T, name, raw_type>(schemaView.from_node(header));
                header["required"] = false;
            }
        }

        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> == source_type::query)
        void append_response_field(reflective::schema_view schemaView) {
            static_assert(uopenapi::http::field_source<T, name> != source_type::query, "query cannot be in response");
        }
        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> == source_type::cookie)
        void append_response_field(reflective::schema_view schemaView) {
            static_assert(uopenapi::http::field_source<T, name> != source_type::query, "openapi 3.0 didnt support cookie in response");
        }

        template<typename T, utils::ce::string name>
        requires (uopenapi::http::field_source<T, name> == source_type::body)
        void append_response_field(reflective::schema_view schemaView) {
            using F = pfr_extension::tuple_element_name_t<T, name>;
            auto& [root, cur] = schemaView;
            auto schema = cur["content"]["application/json"]["schema"];
            auto view = schemaView.from_node(schema);

            if constexpr (!utils::is_optional<F>){
                reflective::call_append<T, name, F>(view);
            }
            else {
                using raw_type = utils::optional_getter_t<T>;
                reflective::call_append<T, name, raw_type>(view);
            }
        }

        template <typename T, std::size_t I>
        void append_response(reflective::schema_view schemaView)
        {
            if (!schemaView.cur_place.IsObject()){
                schemaView.cur_place = userver::formats::yaml::Type::kObject;
            }
            auto this_resp = schemaView.cur_place[std::to_string(I)];
            if (this_resp.IsObject()){
                throw std::runtime_error("dublicated code in responses");
            }
            this_resp = userver::formats::yaml::Type::kObject;
            reflective::place_ref_to_response<T>(this_resp);
            userver::formats::yaml::ValueBuilder response =
                    schemaView.root["components"]["responses"][reflective::schema_type_name<T>()];
            if (response.IsObject())
            {
                return;
            }
            response["description"] = "";
            auto viewResponse = schemaView.from_node(response);
            auto visitor = [viewResponse]<typename, typename Info>(){
                append_response_field<T, Info::name>(viewResponse);
            };
            pfr_extension::visit_struct<T>(visitor);
        }


    }
}