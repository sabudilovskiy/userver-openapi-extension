#pragma once
#include <string>
#include <uopenapi/formats/utils.hpp>
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/parse/to.hpp>

namespace userver::formats::parse {

template <typename T, typename Value>
requires uopenapi::utils::has_strong_converter<std::string, T> &&
         uopenapi::formats::userver_formats_value<Value>
T Parse(const Value& value, userver::formats::parse::To<T>) {
    using converter = uopenapi::utils::converter<std::string, T>;
    auto tmp = value.template As<std::string>();
    return converter::convert(tmp);
}
}  // namespace userver::formats::parse
