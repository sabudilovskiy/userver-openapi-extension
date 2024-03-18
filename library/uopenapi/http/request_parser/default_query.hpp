//
// Created by sabudilovskiy on 3/12/24.
//

#pragma once
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <vector>
#include <string_view>

namespace uopenapi::http{
    template <typename F>
    struct request_parser<std::vector<F>, source_type::query>{
        using Field = std::vector<F>;
        static Field missing(std::string_view){
            return {};
        }
        static Field parse(const std::vector<std::string>& queries, std::string_view){
            return utils::converter<std::vector<std::string>, Field>::convert(queries);
        }
    };

    template <typename Field> requires (!utils::is_optional<Field>)
    struct request_parser<Field, source_type::query>{
        static Field missing(std::string_view field_name){
            throw utils::formatted_exception("Not founded queries with name: [{}]", field_name);
        }
        static Field parse(const std::vector<std::string>& queries, std::string_view field_name){
            if (queries.size() != 1){
                throw utils::formatted_exception("Two more queries with name: [{}]", field_name);
            }
            return utils::converter<std::string, Field>::convert(queries[0]);
        }
    };

    template <typename Field> requires (utils::is_optional<Field>)
    struct request_parser<Field, source_type::query> {
        static Field missing(std::string_view) {
            return utils::optional_getter<Field>::make_none();
        }

        static Field parse(const std::vector<std::string>& queries, std::string_view field_name) {
            if (queries.size() != 1){
                throw utils::formatted_exception("Two more queries with name: [{}]", field_name);
            }
            using raw_type = utils::optional_getter_t<Field>;
            return utils::converter<std::string_view, raw_type>::convert(queries[0]);
        }
    };
}