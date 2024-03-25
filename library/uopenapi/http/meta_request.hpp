#pragma once
#include <cstddef>
#include <uopenapi/http/field_source.hpp>
#include <uopenapi/pfr_extension/visit_struct.hpp>

namespace uopenapi::http {
template <typename T>
constexpr std::size_t count_bodies() {
    std::size_t count = 0;
    auto visit_one_field = [&]<typename, typename Info>() {
        if (field_source<T, Info::name> == source_type::body) {
            count++;
        }
    };
    pfr_extension::visit_struct<T>(visit_one_field);
    return count;
}

}  // namespace uopenapi::http