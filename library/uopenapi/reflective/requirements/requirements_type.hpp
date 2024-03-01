//
// Created by sabudilovskiy on 2/28/24.
//

#pragma once

#include <type_traits>
#include <uopenapi/reflective/requirements/none_requirements.hpp>
#include <uopenapi/utils/check_nttp.hpp>

namespace uopenapi::reflective {
template <typename T>
constexpr auto requirements_type = none_requirements{};

template <typename T>
using requirements_type_t = std::remove_cvref_t<decltype(requirements_type<T>)>;

template <typename T>
constexpr bool trivial_requirements_type =
    std::is_same_v<requirements_type_t<T>, none_requirements>;

template <typename T>
concept nttp_requirements_type =
    requires { utils::check_nttp<requirements_type<T>>(); };
}  // namespace uopenapi::reflective