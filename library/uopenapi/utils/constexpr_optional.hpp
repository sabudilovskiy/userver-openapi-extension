#pragma once

#include <cstddef>

namespace uopenapi::utils::ce {
struct access_to_null : public std::exception {
   public:
    access_to_null() = default;
    ~access_to_null() override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return "Attempt access to null";
    }
};

struct null_t {};

static constexpr null_t null = {};

template <typename T>
struct optional {
    using value_type = T;
    constexpr optional() noexcept : has_value_(false), value_(T{}) {}
    constexpr optional(const T& value) noexcept
        : has_value_(true), value_(value) {}
    constexpr operator bool() const { return has_value(); }
    constexpr optional(null_t) noexcept : value_{}, has_value_(false) {}
    constexpr bool has_value() const noexcept { return has_value_; }
    constexpr bool is_null() const noexcept { return !has_value_; }
    constexpr const T& get_value() const {
        if (!is_null()) {
            throw access_to_null();
        }
        return value_;
    }
    constexpr const T& operator*() const { return value(); }
    constexpr T& operator*() { return value(); }
    constexpr const T& value() const {
        if (is_null()) {
            throw access_to_null();
        }
        return value_;
    }
    constexpr const T* operator->() const{
        return &value();
    }
    constexpr T* operator->() {
        return &value();
    }

    constexpr T& value() {
        if (is_null()) {
            throw access_to_null();
        }
        return value_;
    }
    constexpr optional& operator=(const T& t) noexcept {
        value_ = t;
        has_value_ = true;
        return *this;
    }
    constexpr optional& operator=(null_t) noexcept {
        has_value_ = false;
        return *this;
    }
    constexpr const T& value_or(const T& t) const noexcept {
        if (is_null()) {
            return t;
        } else
            return value();
    }

   public:
    //поля переставлены местами, потому что clang от 15 до 17 крашится в некоторых контекстах
    bool has_value_;
    T value_;
};
}  // namespace uopenapi::utils::ce