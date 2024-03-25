#pragma once
#include <uopenapi/http/source_type.hpp>

namespace uopenapi::http {
template <typename T, source_type SourceType>
struct response_serializator {
    // static void serialize(const T& t, response_info& r_info, std::string_view
    // name)
};
}  // namespace uopenapi::http