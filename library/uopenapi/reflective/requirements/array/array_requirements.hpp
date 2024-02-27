#pragma once
#include <uopenapi/utils/constexpr_optional.hpp>

namespace uopenapi::reflective{

    struct array_requirements{
        utils::ce::optional<std::size_t> min_items;
        utils::ce::optional<std::size_t> max_items;
        bool unique_items = false;
    };
}