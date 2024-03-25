#pragma once
#include <uopenapi/utils/name_type.hpp>

namespace uopenapi::reflective {
template <typename T>
std::string schema_type_name() {
    constexpr utils::type_name_info info = utils::get_type_name_info<T>();
    static_assert(!utils::has_anonymous_namespace<T>(),
                  "overload schema_type_name() if your type placed in "
                  "anonymous namespaces");
    static_assert(!utils::is_template<T>(),
                  "overload schema_type_name() if your type placed in "
                  "anonymous namespaces");
    constexpr std::size_t N = decltype(info)::count_ns;
    std::size_t need_len = N;
    for (std::size_t i = 0; i < N; i++) {
        need_len += info.namespaces[i].size();
    }
    need_len += info.name.size();
    std::string result;
    result.reserve(need_len);
    for (std::size_t i = 0; i < N; i++) {
        result += info.namespaces[i];
        result += ".";
    }
    result += info.name;
    return result;
}
}  // namespace uopenapi::reflective
