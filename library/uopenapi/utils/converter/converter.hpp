#pragma once

namespace uopenapi::utils {

template <typename From, typename To>
struct converter {
    using source_type = From;
    using result_type = To;
    // static result_type convert(const source_type&) = delete;
};

template <typename From, typename To>
concept can_convert = requires(const From& from) {
    { converter<From, To>::convert(from) } -> std::convertible_to<To>;
};
}  // namespace uopenapi::utils
