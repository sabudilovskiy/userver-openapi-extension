#pragma once
#include <type_traits>

namespace uopenapi::utils {

template <typename T, T v>
struct nttp_adl {
    static constexpr T value = v;
};

template <auto v>
auto create_nttp_adl() {
    return nttp_adl<std::remove_cvref_t<decltype(v)>, v>{};
}
}  // namespace uopenapi::utils
