#pragma once
#include <compare>
#include <uopenapi/reflective/alias/alias_meta.hpp>
#include <uopenapi/reflective/requirements/requirements_field.hpp>
#include <uopenapi/utils/forward_like.hpp>
#include <utility>

namespace uopenapi::reflective {
template <typename U>
struct Alias {
    using base_type = U;

    explicit Alias(U u) : value_(std::move(u)) {}

    template <typename... T>
    requires std::is_constructible_v<U, T...>
    Alias(T&&... t) : value_(std::forward<T>(t)...) {}

    auto& operator=(U u) {
        value_ = std::move(u);
        return *this;
    }

    template <typename T>
    requires std::is_assignable_v<U, T>
    auto& operator=(T&& t) {
        value_ = std::forward<T>(t);
    }

    U* operator->() { return std::addressof(value_); }

    const U* operator->() const { return std::addressof(value_); }

    auto* get_address() { return &value_; }

    auto& operator*() { return value_; }

    auto* get_address() const { return &value_; }

    auto& operator*() const { return value_; }

    auto operator<=>(const Alias<U>& rhs) const = default;

    auto operator<=>(const U& rhs) requires std::three_way_comparable<U>
    {
        return value_ <=> rhs;
    }

   private:
    U value_;
};

template <typename T>
requires std::is_base_of_v<Alias<typename T::basic_type>,
                           std::remove_cvref_t<T>>
struct is_alias<T> : std::true_type {};

template <typename T>
requires std::is_base_of_v<Alias<typename T::basic_type>,
                           std::remove_cvref_t<T>>
struct alias_getter<T> {
    decltype(auto) get(T t) { return utils::forward_like<T>(*t); }
};

template <typename T, auto req>
requires is_alias_v<T>
auto validate(T&& t) {
    using req_type = decltype(req);
    static_assert(std::is_same_v<req_type, none_requirements>,
                  "alias must be without requirements on field");
    return validate<req>(alias_getter<T>(std::forward<T>(t)));
}

template <typename T, typename Requirements>
requires is_alias_v<T>
auto validate(T&&, Requirements) {
    using req_type = std::remove_cvref_t<Requirements>;
    static_assert(std::is_same_v<req_type, none_requirements>,
                  "alias must be without requirements on field");
}
}  // namespace uopenapi::reflective
