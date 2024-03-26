#pragma once

#pragma once
#include <limits>
#include <string_view>
#include <type_traits>
#include <uopenapi/enum/introspector.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi {
namespace detail {
template <utils::ce::string str>
constexpr auto count_borders() {
    std::size_t res = 0;
    for (char ch : str.AsStringView()) {
        if (ch == ',') res++;
    }
    return res;
}

template <utils::ce::string str>
constexpr auto split_tokens() {
    constexpr std::size_t token_quantity = count_borders<str>() + 1;
    std::string_view sv = str.AsStringView();
    std::array<std::string_view, token_quantity> res;
    std::size_t cur_index = 0;
    std::size_t cur_begin = 0;
    std::size_t cur_end = 0;
    while (cur_index != token_quantity) {
        cur_end = sv.find_first_of(',', cur_begin);
        res[cur_index] = sv.substr(cur_begin, cur_end - cur_begin);
        cur_begin = cur_end + 2;
        cur_index++;
    }
    return res;
}

constexpr bool IsDigit(char ch) { return '0' <= ch && ch <= '9'; }

constexpr int ParseDigit(char ch) {
    switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return ch - '0';
        default:
            throw -1;
    }
}

template <typename T>
requires std::is_integral_v<T>
constexpr T ParseIntegral(std::string_view sv) {
    bool is_positive = true;
    bool has_sign = false;
    auto max_prev = std::numeric_limits<T>::max() / 10;
    T result{};
    for (char ch : sv) {
        if (ch == '-' && !has_sign) {
            has_sign = true;
            is_positive = false;
            if (std::numeric_limits<T>::min() == 0) {
                return result;
            }
        } else if (ch == '+' && !has_sign) {
            has_sign = true;
        } else if (IsDigit(ch)) {
            auto add = ParseDigit(ch);
            if (max_prev > result) {
                result = result * 10 + add;
            } else {
                return result;
            }
        } else {
            return result;
        }
    }
    if (!is_positive) {
        result = -result;
    }
    return result;
}

constexpr std::string_view GetPureWord(std::string_view token) {
    size_t begin = token.find_first_not_of(' ');
    size_t end = token.find_last_not_of(begin, ' ');
    if (end != std::string_view::npos && token[end] == ' ') end--;
    size_t size = (end == std::string_view ::npos) ? end : end - begin + 1;
    return token.substr(begin, size);
}

template <typename T, std::size_t N>
constexpr std::array<T, N> get_values(std::array<std::string_view, N> tokens) {
    using Raw = std::underlying_type_t<T>;
    Raw last = 0;
    std::array<T, N> values;
    std::size_t index = 0;
    for (auto token : tokens) {
        auto eq = token.find('=');
        if (eq != std::string_view::npos) {
            auto value_sv = GetPureWord(token.substr(eq + 1));
            last = ParseIntegral<Raw>(value_sv);
        }
        values[index] = static_cast<T>(last);
        last++;
        index++;
    }
    return values;
}

constexpr bool IsLiteral(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

template <std::size_t N>
constexpr std::array<std::string_view, N> get_names(
    std::array<std::string_view, N> tokens) {
    std::array<std::string_view, N> names;
    std::size_t index = 0;
    for (std::string_view token : tokens) {
        auto eq = token.find('=');
        if (eq != std::string_view::npos) {
            names[index] = GetPureWord(token.substr(0, eq));
        } else {
            names[index] = GetPureWord(token);
        }
        index++;
    }
    return names;
}

}  // namespace detail

}  // namespace uopenapi

#define UOPENAPI_DECLARE_ENUM(NAME, TYPE, ...)                              \
    enum struct NAME : TYPE { __VA_ARGS__ };                                \
    struct enum_introspector_##NAME;                                        \
    consteval enum_introspector_##NAME* get_enum_introspector(              \
        std::type_identity<NAME>) {                                         \
        return nullptr;                                                     \
    }                                                                       \
    struct enum_introspector_##NAME {                                       \
        static constexpr auto tokens =                                      \
            ::uopenapi::detail::split_tokens<#__VA_ARGS__>();               \
        static constexpr auto names = ::uopenapi::detail::get_names(tokens); \
        static constexpr auto values =                                      \
            ::uopenapi::detail::get_values<NAME>(tokens);                   \
    }
