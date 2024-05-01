#pragma once

#include <string_view>
#include <uopenapi/utils/formatted_exception.hpp>
namespace uopenapi::http::details {
inline auto missing_header(std::string_view fieldName) {
    return utils::formatted_exception("Not founded header with name: [{}]",
                                      fieldName);
}
}  // namespace uopenapi::http::details
