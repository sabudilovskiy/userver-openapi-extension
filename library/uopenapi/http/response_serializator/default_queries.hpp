#pragma once
#include <uopenapi/http/response_info.hpp>
#include <uopenapi/http/response_serializator/serializator.hpp>
#include <uopenapi/utils/converter/string/string.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/optional_meta/is_optional.hpp>
#include <uopenapi/utils/optional_meta/optional_getter.hpp>

namespace uopenapi::http {
template <typename T>
struct response_serializator<T, source_type::query> {
    static void serialize(const T&, response_info&, std::string_view) {
        static_assert(sizeof(T) == 0, "Query cannot be used in responses");
    }
};
}  // namespace uopenapi::http
