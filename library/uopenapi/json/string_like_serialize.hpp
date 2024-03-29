#pragma once
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace userver::formats::json {

template <typename T>
requires uopenapi::utils::can_convert<T, std::string>
userver::formats::json::Value Serialize(
    const T& value,
    userver::formats::serialize::To<userver::formats::json::Value>) {
    using converter = uopenapi::utils::converter<T, std::string>;
    return userver::formats::json::ValueBuilder{converter::convert(value)}
        .ExtractValue();
}
}  // namespace userver::formats::json
