#pragma once
#include <boost/optional/optional.hpp>
#include <uopenapi/utils/is_optional/is_optional.hpp>

namespace uopenapi::utils {
template <typename T>
constexpr bool is_optional<boost::optional<T>> = true;
}