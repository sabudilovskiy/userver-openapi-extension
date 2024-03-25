#pragma once
#include <uopenapi/http/code.hpp>

namespace uopenapi::http {
template <typename T, std::size_t StatusCode = 200>
struct response {
    using value_type = T;
    static constexpr userver::server::http::HttpStatus code =
        status_code_v<StatusCode>;
    template <typename... Args>
    response(Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args&&...>)
        : value(std::forward<Args>(args)...) {}
    template <typename Arg>
    response(std::initializer_list<Arg> init_list) noexcept(
        std::is_nothrow_constructible_v<value_type, std::initializer_list<Arg>>)
        : value(std::move(init_list)) {}
    template <class Arg>
    auto& operator=(Arg&& arg) {
        value = std::forward<Arg>(arg);
        return value;
    }
    auto& operator=(value_type rhs) {
        value = std::move(rhs);
        return *this;
    }
    value_type& operator()() noexcept { return value; }
    const value_type& operator()() const noexcept { return value; }

   public:
    T value;
};
}  // namespace uopenapi::http