#pragma once

#include <experimental/mdspan>


namespace stdex = std::experimental;


template<size_t N>
using extents = stdex::dextents<size_t, N>;



size_t flat_size(auto ext){

    size_t ret(1);
    for (size_t i = 0; i < ext.rank(); ++i){
        ret *= ext.extent(i);
    }
    return ret;

}