#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <type_traits>
#include <uopenapi/utils/converter/converter.hpp>

namespace uopenapi::utils {
template <>
struct converter<boost::uuids::uuid, std::string> {
    static std::string convert(const boost::uuids::uuid& value) {
        return boost::uuids::to_string(value);
    }
};
}  // namespace uopenapi::utils
