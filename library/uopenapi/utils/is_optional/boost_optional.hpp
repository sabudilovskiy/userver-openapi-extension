#pragma once
#include <uopenapi/utils/is_optional/is_optional.hpp>
#include <boost/optional/optional.hpp>

namespace uopenapi::utils{
    template <typename T>
    constexpr bool is_optional<boost::optional<T>> = true;
}