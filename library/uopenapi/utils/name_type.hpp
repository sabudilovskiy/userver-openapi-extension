#include <boost/current_function.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

#pragma once

namespace uopenapi::utils {

namespace details {
constexpr std::string_view anonymous_namespace() {
#if defined(__clang__)
    return "(anonymous namespace)";
#elif defined(__GNUC__)
    return "{anonymous}";
#else
#error("unsupported compiler")
#endif
}

template <std::size_t N>
constexpr std::string_view get_clear_name(std::string_view raw_name) {
#if defined(__clang__)
    return raw_name.substr(12 + N, raw_name.size() - 13 - N);
#elif defined(__GNUC__)
    return raw_name.substr(27 + N, raw_name.size() - 28 - N);
#else
#error("unsupported compiler")
#endif
}
}  // namespace details

constexpr std::string_view anonymous_ns = details::anonymous_namespace();

template <std::size_t N>
struct type_name_info {
    static constexpr auto count_ns = N;
    std::array<std::string_view, N> namespaces{};
    std::string_view name{};
};

template <typename T>
constexpr auto get_full_name_type() {
    constexpr std::string_view raw_name = BOOST_CURRENT_FUNCTION;
    constexpr auto clear_name =
        details::get_clear_name<sizeof("uopenapi::utils::get_full_name_type")>(
            raw_name);
    return clear_name;
}

template <typename T>
constexpr auto get_type_name_info() {
    constexpr auto name = get_full_name_type<T>();
    constexpr auto open_template = std::find(name.begin(), name.end(), '<');
    constexpr std::size_t count_namespaces =
        std::count(name.begin(), open_template, ':') / 2;
    type_name_info<count_namespaces> info;
    std::size_t cur_begin = 0;
    std::size_t last;
    for (std::size_t i = 0; i < count_namespaces; i++) {
        last = name.find(':', cur_begin);
        info.namespaces[i] = name.substr(cur_begin, last - cur_begin);
        cur_begin = last + 2;
    }
    info.name = name.substr(cur_begin);
    return info;
}

template <typename T>
constexpr bool has_anonymous_namespace() {
    constexpr auto info = get_type_name_info<T>();
    return std::find(info.namespaces.begin(), info.namespaces.end(),
                     anonymous_ns) != info.namespaces.end();
}

template <typename T>
constexpr bool is_template() {
    constexpr auto info = get_type_name_info<T>();
    return std::find(info.name.begin(), info.name.end(), '<') !=
           info.name.end();
}
}  // namespace uopenapi::utils
