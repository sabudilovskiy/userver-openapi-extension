#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <type_traits>
#include <uopenapi/utils/converter/converter.hpp>

namespace uopenapi::utils {
template <>
struct converter<std::string, boost::uuids::uuid> {
    static boost::uuids::uuid convert(const std::string& str) {
        return boost::lexical_cast<boost::uuids::uuid>(str);
    }
};
template <>
struct converter<std::string_view, boost::uuids::uuid> {
    static boost::uuids::uuid convert(std::string_view str) {
        return boost::lexical_cast<boost::uuids::uuid>(str);
    }
};
}  // namespace uopenapi::utils
