#pragma once

namespace uopenapi::utils {

enum convert_type {
    none,
    weak,   // string -> int
    strong  // string -> enum
};

template <typename From, typename To>
struct converter {
    static constexpr auto type = convert_type::none;
    // static To convert(const From&) = delete;
};

template <typename From, typename To>
constexpr auto converter_type = converter<From, To>::type;

template <typename From, typename To>
constexpr bool has_strong_converter =
    converter_type<From, To> == convert_type::strong;

template <typename From, typename To>
constexpr bool has_weak_converter =
    converter_type<From, To> == convert_type::weak;

template <typename From, typename To>
constexpr bool has_any_converter =
    converter_type<From, To> != convert_type::none;
}  // namespace uopenapi::utils
