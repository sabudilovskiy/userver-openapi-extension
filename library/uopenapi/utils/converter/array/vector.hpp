#pragma once
#include <uopenapi/utils/converter/converter.hpp>
#include <vector>

namespace uopenapi::utils {
template <typename From, typename To>
struct converter<std::vector<From>, std::vector<To>> {
    static std::vector<To> convert(const std::vector<From>& from) {
        std::vector<To> result;
        result.reserve(from.size());
        for (auto& from_item : from) {
            result.emplace_back(converter<From, To>::convert(from_item));
        }
        return result;
    }
};
}  // namespace uopenapi::utils
