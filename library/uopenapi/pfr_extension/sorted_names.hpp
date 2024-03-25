#pragma once
#include <algorithm>
#include <array>
#include <boost/pfr.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::pfr_extension {
namespace details {
template <typename Type, std::size_t... sizes>
constexpr auto array_cat(const std::array<Type, sizes>&... arrays) {
    std::array<Type, (sizes + ...)> result;
    std::size_t index{};

    ((std::copy_n(arrays.begin(), sizes, result.begin() + index),
      index += sizes),
     ...);

    return result;
}
}  // namespace details
template <typename T>
consteval auto get_sorted_names() {
    using without_ref = std::remove_cvref_t<T>;
    auto names = boost::pfr::names_as_array<without_ref>();
    std::sort(names.begin(), names.end());
    return names;
}

template <typename T>
consteval auto get_sorted_names_with_indexes() {
    using without_ref = std::remove_cvref_t<T>;
    constexpr auto names = boost::pfr::names_as_array<without_ref>();
    std::array<std::pair<std::string_view, std::size_t>,
               boost::pfr::tuple_size_v<T>>
        names_pair{};
    for (std::size_t i = 0; i < names.size(); i++) {
        names_pair[i] = {names[i], i};
    }
    std::sort(names_pair.begin(), names_pair.end(),
              [](const auto& left, const auto& right) {
                  return left.first < right.first;
              });
    return names_pair;
}

namespace details {
template <typename... T>
constexpr auto get_result_with_nulls() {
    auto all_names = array_cat(get_sorted_names<T>()...);
    std::sort(all_names.begin(), all_names.end());
    auto size =
        std::unique(all_names.begin(), all_names.end()) - all_names.begin();
    return std::make_pair(all_names, size);
}
}  // namespace details

template <typename... T>
constexpr auto get_size_set_of_names() {
    return details::get_result_with_nulls<T...>().second;
}

template <typename... T>
constexpr auto get_set_of_names() {
    constexpr auto result_with_nulls_with_size =
        details::get_result_with_nulls<T...>();
    constexpr auto N = result_with_nulls_with_size.second;
    std::array<std::string_view, N> result{};
    for (std::size_t i = 0; i < N; i++) {
        result[i] = result_with_nulls_with_size.first[i];
    }
    return result;
}
}  // namespace uopenapi::pfr_extension
