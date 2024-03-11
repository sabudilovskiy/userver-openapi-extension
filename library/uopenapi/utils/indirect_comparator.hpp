#pragma once

namespace uopenapi::utils {
template <typename T>
struct indirect_comparator {
    bool operator()(const T& left, const T& right) const {
        if (!left && !right) {
            return true;
        }
        if (!left || !right) {
            return false;
        }
        return *left == *right;
    }
};
}  // namespace uopenapi::utils