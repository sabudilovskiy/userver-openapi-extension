#pragma once
#include <algorithm>
#include <unordered_set>
#include <uopenapi/reflective/requirements/array/array_requirements.hpp>
#include <uopenapi/reflective/validate_result.hpp>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/indirect_comparator.hpp>
#include <uopenapi/utils/indirect_hash.hpp>
#include <vector>

namespace uopenapi::reflective {
template <typename T, array_requirements req>
validate_result validate(const std::vector<T>& value, utils::nttp_adl<array_requirements, req>) {
    if (req.min_items && *req.min_items > value.size()) {
        return validate_result::error(
            "array has size: {} less than min_items: {}", value.size(),
            *req.min_items);
    }
    if (req.max_items && *req.max_items < value.size()) {
        return validate_result::error(
            "array has size: {} greater than max_items: {}", value.size(),
            *req.min_items);
    }
    if constexpr (req.unique_items) {
        using comparator = utils::indirect_comparator<T*>;
        using hash = utils::indirect_hash<T*, std::hash<T>>;
        std::unordered_set<T*, hash, comparator> hash_set;
        std::size_t index = 0;
        for (auto& item : value) {
            auto ptr = std::addressof(item);
            auto [it, emplaced] = hash_set.emplace(ptr, index);
            if (!emplaced) {
                auto first_index =
                    std::find(value.begin(), value.end(), item) - value.begin();
                auto second_index = &item - value.data();
                return validate_result::error(
                    "array has duplicated. first_index: {}, second_index: {}",
                    first_index, second_index);
            }
            index++;
        }
    }
    return validate_result::ok();
}
}  // namespace uopenapi::reflective