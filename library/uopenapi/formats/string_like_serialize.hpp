#pragma once
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/serialize/to.hpp>

namespace userver::formats::json {

template <typename T, typename Value>
requires uopenapi::utils::can_convert<T, std::string>
Value Serialize(const T& value, userver::formats::serialize::To<Value>) {
    using converter = uopenapi::utils::converter<T, std::string>;
    return typename Value::Builder{converter::convert(value)}.ExtractValue();
}
}  // namespace userver::formats::json
