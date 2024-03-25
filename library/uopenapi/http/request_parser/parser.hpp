//
// Created by sabudilovskiy on 3/12/24.
//

#pragma once
#include <uopenapi/http/source_type.hpp>

namespace uopenapi::http {
template <typename T, source_type sourceType>
struct request_parser {
    // static T missing(std::string_view name)
    // static T parse(some_type value, std::string_view field_name)
};

}  // namespace uopenapi::http