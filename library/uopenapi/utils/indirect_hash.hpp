#pragma once
#include <cstdint>

namespace uopenapi::utils {
template <typename T, typename Hash>
struct indirect_hash {
    std::size_t operator()(const T& t) {
        if (!t) {
            return 0;
        }
        return Hash{}(*t);
    }
};
}  // namespace uopenapi::utils