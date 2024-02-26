#pragma once
#include <optional>
#include <numeric>
#include <uopenapi/utils/formatted_exception.hpp>
#include <uopenapi/utils/constexpr_optional.hpp>

namespace uopenapi::reflective {


    template <typename T>
    struct integer_requirements {
        utils::ce::optional<T> minimum;
        utils::ce::optional<T> maximum;
        utils::ce::optional<bool> exclusive_minimum;
        utils::ce::optional<bool> exclusive_maximum;
        utils::ce::optional<T> multiple_of;
    };

    template <integer_requirements req, typename T>
    void validate(const T& value){
        if (req.minimum){
            if (value < *req.minimum ){
                throw utils::formated_exception("value: {} less than minumum: {}", value, *req.minimum);
            }
            else if (value == *req.minimum && req.exclusive_minimum.value_or(false)){
                throw utils::formated_exception("value: {} must be greater: {}", value, *req.minimum);
            }
        }
        if (req.maximum){
            if (value > *req.maximum ){
                throw utils::formated_exception("value: {} greater than maximum: {}", value, *req.minimum);
            }
            else if (value == *req.maximum && req.exclusive_maximum.value_or(false)){
                throw utils::formated_exception("value: {} must be greater: {}", value, *req.minimum);
            }
        }
        if (req.multiple_of){
            if constexpr (std::is_floating_point_v<T>){
                auto mod = std::abs(std::fmod(value, *req.multiple_of));
                if (mod < std::numeric_limits<T>::epsilon()){
                    throw utils::formated_exception("value: {} must be multiplyOf: {}", value, *req.minimum);
                }
            }
            else {
                if (value % *req.multiple_of != 0){
                    throw utils::formated_exception("value: {} must be multiplyOf: {}", value, *req.minimum);
                }
            }
        }
    }
}