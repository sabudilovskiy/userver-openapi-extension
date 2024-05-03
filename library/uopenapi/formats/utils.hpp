#pragma once

namespace uopenapi::formats {
template <typename Value>
concept userver_formats_value = requires { typename Value::Builder; };
}
