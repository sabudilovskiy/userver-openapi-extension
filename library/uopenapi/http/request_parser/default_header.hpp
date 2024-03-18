//
// Created by sabudilovskiy on 3/12/24.
//

#pragma once
#include <uopenapi/http/request_parser/parser.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <string_view>

namespace uopenapi::http{
    template <typename Field> requires (!utils::is_optional<Field>)
    struct request_parser<Field, source_type::header>{
        static Field missing(std::string_view field_name){
            throw utils::formatted_exception("Not founded header with name: [{}]", field_name);
        }
        static Field parse(std::string_view text, std::string_view){
            return utils::converter<std::string_view, Field>::convert(text);
        }
    };

    template <typename Field> requires (utils::is_optional<Field>)
    struct request_parser<Field, source_type::header> {
        static Field missing(std::string_view) {
            return utils::optional_getter<Field>::make_none();
        }

        static Field parse(std::string_view text, std::string_view) {
            using raw_type = utils::optional_getter_t<Field>;
            return utils::converter<std::string_view, raw_type>::convert(text);
        }
    };
}