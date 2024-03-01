#pragma once
#include <fmt/format.h>

#include <string>

namespace uopenapi::reflective {
template <typename T>
concept is_validate_result = requires(T&& t, void (*some_bool_function)(bool)) {
                                 { some_bool_function((T &&) t) };
                                 { std::forward<T>(t).error_message() };
                             };
struct validate_result {
    static validate_result error(std::exception& exc) {
        return validate_result{.has_error_ = true,
                               .error_message_ = exc.what()};
    }
    template <typename... T>
    static validate_result error(fmt::format_string<T...> str, T&&... args) {
        return validate_result{.has_error_ = true,
                               .error_message_ = fmt::format(
                                   std::move(str), std::forward<T>(args)...)};
    }
    static validate_result ok() { return validate_result{.has_error_ = false}; }
    operator bool() const { return !has_error_; }
    auto&& error_message() & { return error_message_; }

    auto&& error_message() const& { return error_message_; }

    auto error_message() && { return std::move(error_message_); }

   public:
    bool has_error_ = false;
    std::string error_message_;
};

static_assert(is_validate_result<validate_result>);
}  // namespace uopenapi::reflective