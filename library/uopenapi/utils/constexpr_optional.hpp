#pragma once

#include <cstddef>
#include <optional>
#include <string_view>

namespace uopenapi::utils{
    struct access_to_null : public std::exception
    {
    public:
        access_to_null() = default;
        ~access_to_null() override = default;

        [[nodiscard]] const char* what() const noexcept override
        {
            return "Attempt access to null";
        }
    };

    struct null_t
    {
    };

    static constexpr null_t null = {};

    template <typename T>
    struct constexpr_optional
    {
        using value_type = T;
        constexpr constexpr_optional() noexcept : value_(T{}), has_value_(false)
        {
        }
        constexpr constexpr_optional(const T& value) noexcept
                : value_(value), has_value_(true)
        {
        }
        constexpr constexpr_optional(null_t) noexcept
                : value_{}, has_value_(false)
        {
        }
        constexpr bool has_value() const noexcept
        {
            return has_value_;
        }
        constexpr bool is_null() const noexcept
        {
            return !has_value_;
        }
        constexpr const T& get_value() const
        {
            if (!is_null())
            {
                throw access_to_null();
            }
            return value_;
        }
        constexpr const T& operator*() const
        {
            return value();
        }
        constexpr const T& operator*()
        {
            return value();
        }
        constexpr const T& value() const
        {
            if (is_null())
            {
                throw access_to_null();
            }
            return value_;
        }
        constexpr T& value()
        {
            if (is_null())
            {
                throw access_to_null();
            }
            return value_;
        }
        constexpr constexpr_optional& operator=(const T& t) noexcept
        {
            value_ = t;
            has_value_ = true;
            return *this;
        }
        constexpr constexpr_optional& operator=(null_t) noexcept
        {
            has_value_ = false;
            return *this;
        }
        constexpr const T& value_or(const T& t) const noexcept
        {
            if (is_null())
            {
                return t;
            }
            else
                return value();
        }
    public:
        T value_;
        bool has_value_;
    };
}