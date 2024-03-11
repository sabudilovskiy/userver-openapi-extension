#pragma once
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/http/response_info.hpp>
#include <uopenapi/http/meta_request.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <userver/formats/json.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/reflective/requirements/validate.hpp>
#include <uopenapi/http/response.hpp>

namespace uopenapi::http{
    namespace details{
        template<typename T>
        response_info make_base_response_info(){
            static_assert(count_bodies<T>() <= 1, "maximum one body in request");
            if constexpr (count_bodies<T>() == 1){
                return response_info{
                    .content_type = userver::http::content_type::kApplicationJson
                };
            }
            else if constexpr (count_bodies<T>() == 0){
                return response_info{
                    .content_type = userver::http::content_type::kTextPlain
                };
            }
        }

        template<typename T>
        std::string serialize_json_body(const T& body) {
            return ToString(userver::formats::json::ValueBuilder{body}.ExtractValue());
        }

        template<typename F>
        std::string serialize_as_string(const F& f) {
            static_assert(utils::can_convert<F, std::string>, "Must exist converter from your type to string");
            return utils::converter<F, std::string>::convert(f);
        }
    }


    template <typename T>
    response_info serialize_response_info(const T& t){
        response_info res = details::make_base_response_info<T>();
        auto visit_all = [&]<typename Info, typename F>(const F& f){
            constexpr source_type st = field_source<T, Info::name>;
            if constexpr (st == source_type::body){
                res.body = details::serialize_json_body(f);
            }
            else if constexpr (st == source_type::cookie){
                res.cookies[Info::name.AsStringView()] = details::serialize_as_string(f);
            }
            else if constexpr (st == source_type::header){
                res.headers[Info::name.AsStringView()] = details::serialize_as_string(f);
            }
            else if constexpr (st == source_type::query){
                static_assert(st != source_type::query, "queries cannot be used in responses");
            }
            else {
                static_assert(st == source_type::body, "unreachable");
            }
        };
        pfr_extension::for_each_named_field(t, visit_all);
        return res;
    }

    template <typename T, std::size_t Code>
    response_info serialize_response_info(const response<T, Code>& resp) {
        auto res = serialize_response_info(resp.value);
        res.status_code = resp.code;
        return res;
    }
}
