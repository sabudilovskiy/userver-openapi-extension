#pragma once

namespace uopenapi::utils{

    template <typename T, T v>
    struct nttp_adl{
        static constexpr T value = v;
    };

    template <auto v>
    auto create_nttp_adl(){
        return nttp_adl<decltype(v), v>{};
    }
}