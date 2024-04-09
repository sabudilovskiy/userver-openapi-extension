#pragma once
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/parse/to.hpp>

namespace userver::formats::parse {

template <typename T, typename Value>
requires uopenapi::utils::can_convert<std::string, T>
T Parse(const Value& value, userver::formats::parse::To<T>) {
    using converter = uopenapi::utils::converter<std::string, T>;
    auto tmp = value.template As<std::string>();
    return converter::convert(tmp);
}
}  // namespace userver::formats::parse
