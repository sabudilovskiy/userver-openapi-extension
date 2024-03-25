#pragma once
#include <fmt/format.h>

#include <exception>
#include <stdexcept>
#include <utility>

namespace uopenapi::utils {
/*
Небольшая обёртка, чтобы не писать каждый раз fmt::format()
*/
struct formatted_exception : std::runtime_error {
    /*
        конструктор форвардит внутрь фмт формат
    */
    template <typename... T>
    formatted_exception(fmt::format_string<T...> fmt, T&&... args)
        : std::runtime_error(fmt::format(fmt, std::forward<T>(args)...)) {}
};
}  // namespace uopenapi::utils
