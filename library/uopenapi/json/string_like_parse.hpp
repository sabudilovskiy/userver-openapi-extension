#pragma once
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/json/value.hpp>

namespace userver::formats::json {

template <typename T>
requires uopenapi::utils::can_convert<std::string, T>
T Parse(const formats::json::Value& value, userver::formats::parse::To<T>) {
    using converter = uopenapi::utils::converter<std::string, T>;
    auto tmp = value.As<std::string>();
    return converter::convert(tmp);
}
}  // namespace userver::formats::json
