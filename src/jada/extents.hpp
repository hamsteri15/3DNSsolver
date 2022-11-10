#pragma once

#include <experimental/mdspan>
#include <array>
#include "jada/rank.hpp"
#include "jada/utils.hpp"


namespace stdex = std::experimental;


template<size_t N>
using extents = stdex::dextents<size_t, N>;



static auto make_extent(auto dims){
    return extents<rank(dims)>{dims};
}



static auto extent_to_array(auto ext){

    static constexpr size_t N = decltype(ext)::rank();
    using idx_t = typename decltype(ext)::index_type;

    std::array<idx_t, N> ret{};
    for (size_t i = 0; i < N; ++i){
        ret[i] = ext.extent(i);
    }

    return ret;
}


static constexpr size_t flat_size(auto ext){

    size_t ret(1);
    for (size_t i = 0; i < ext.rank(); ++i){
        ret *= ext.extent(i);
    }
    return ret;

}


bool indices_in_bounds(auto indices, auto extents){

    static constexpr size_t N = decltype(extents)::rank();
    auto inds = tuple_to_array(indices);

    for (size_t i = 0; i < N; ++i){
        if (inds[i] >= extents.extent(i)){
            return false;
        }
    }
    return true;

}

