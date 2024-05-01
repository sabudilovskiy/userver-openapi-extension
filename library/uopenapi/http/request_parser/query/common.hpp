#pragma once

#include <string>
#include <uopenapi/utils/formatted_exception.hpp>
#include <vector>

namespace uopenapi::http::details {
inline auto missing_queries(std::string_view fieldName) {
    return utils::formatted_exception("Not founded queries with name: [{}]",
                                      fieldName);
}
inline void assert_queries_quantity(const std::vector<std::string>& queries,
                                    std::string_view fieldName) {
    if (queries.size() != 1) {
        throw utils::formatted_exception("Two more queries with name: [{}]",
                                         fieldName);
    }
}
}  // namespace uopenapi::http::details
