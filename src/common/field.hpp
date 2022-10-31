#pragma once

#include <concepts>
#include "common/allocator.hpp"
#include "topaz/include/all.hpp"
#include "common/vector.hpp"
#include "common/scalar.hpp"

template<class ET>
using Field = topaz::NumericArray<ET, allocator_t<ET>>;


template<class T>
concept field_c = topaz::IsRangeOrNumericArray_v<T>;



static auto serialize(const Field<scalar>& f){
    
    return std::vector<scalar>(f.begin(), f.end());
}

template<size_t L>
static auto serialize(const Field<Vector<L>>& f){

    size_t n = f.size() * L;

    std::vector<scalar> ret(n);

    for (size_t i = 0; i < f.size(); ++i){

        for (size_t j = 0; j < L; ++j){
            ret[i*L + j] = f[i][j];
        }
    }
    return ret;


}