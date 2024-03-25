#pragma once
#include <numeric>
#include <optional>
#include <uopenapi/reflective/validate_result.hpp>
#include <uopenapi/utils/constexpr_optional.hpp>
#include <uopenapi/utils/nttp_adl.hpp>

namespace uopenapi::reflective {

template <typename T>
struct number_requirements {
    utils::ce::optional<T> minimum;
    utils::ce::optional<T> maximum;
    // exclusiveMinimum
    bool exclusive_minimum = false;
    // exclusiveMaximum
    bool exclusive_maximum = false;
    // multipleOf
    utils::ce::optional<T> multiple_of;
};

template <typename T, number_requirements<T> req>
validate_result validate(const T& value,
                         utils::nttp_adl<number_requirements<T>, req>) {
    if (req.minimum) {
        if (value < *req.minimum) {
            return validate_result::error("value: [{}] less than minumum: [{}]",
                                          value, *req.minimum);
        } else if (value == *req.minimum && req.exclusive_minimum) {
            return validate_result::error(
                "value: [{}] must be greater than exclusive minimum: [{}]",
                value, *req.minimum);
        }
    }
    if (req.maximum) {
        if (value > *req.maximum) {
            return validate_result::error(
                "value: [{}] greater than maximum: [{}]", value, *req.maximum);
        } else if (value == *req.maximum && req.exclusive_maximum) {
            return validate_result::error(
                "value: [{}] must be less than exclusive maximum: [{}]", value,
                *req.maximum);
        }
    }
    if (req.multiple_of) {
        if constexpr (std::is_floating_point_v<T>) {
            auto mod = std::abs(std::fmod(value, *req.multiple_of));
            if (mod > std::numeric_limits<T>::epsilon()) {
                return validate_result::error(
                    "value: [{}] must be multiplyOf: [{}]", value,
                    *req.multiple_of);
            }
        } else {
            if (value % *req.multiple_of != 0) {
                return validate_result::error(
                    "value: [{}] must be multiplyOf: [{}]", value,
                    *req.multiple_of);
            }
        }
    }
    return validate_result::ok();
}
}  // namespace uopenapi::reflective