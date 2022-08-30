#pragma once

#include <experimental/mdspan>
#include <array>

namespace stdex = std::experimental;


template<size_t N>
using extents = stdex::dextents<size_t, N>;

auto extent_to_array(auto ext){

    static constexpr size_t N = decltype(ext)::rank();
    using idx_t = typename decltype(ext)::index_type;

    std::array<idx_t, N> ret{};
    for (size_t i = 0; i < N; ++i){
        ret[i] = ext.extent(i);
    }

    return ret;
}


size_t flat_size(auto ext){

    size_t ret(1);
    for (size_t i = 0; i < ext.rank(); ++i){
        ret *= ext.extent(i);
    }
    return ret;

}