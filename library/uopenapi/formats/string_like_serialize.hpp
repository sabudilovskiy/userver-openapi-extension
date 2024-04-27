#pragma once
#include <string>
#include <string_view>
#include <uopenapi/utils/converter/converter.hpp>
#include <userver/formats/serialize/to.hpp>

namespace userver::formats::serialize {

template <typename T, typename Value>
requires(uopenapi::utils::has_strong_converter<T, std::string> ||
         uopenapi::utils::has_strong_converter<T, std::string_view>)
Value Serialize(const T& value, userver::formats::serialize::To<Value>) {
    if constexpr (uopenapi::utils::has_strong_converter<T, std::string_view>) {
        using converter = uopenapi::utils::converter<T, std::string_view>;
        return
            typename Value::Builder{converter::convert(value)}.ExtractValue();
    } else {
        using converter = uopenapi::utils::converter<T, std::string>;
        return
            typename Value::Builder{converter::convert(value)}.ExtractValue();
    }
}
}  // namespace userver::formats::serialize
