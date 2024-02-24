#pragma once
#include <string_view>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi::pfr_extension{
    template <std::size_t Index, utils::constexpr_string Name>
    struct field_info{
        static constexpr auto index = Index;
        static constexpr auto name = Name;
    };
}