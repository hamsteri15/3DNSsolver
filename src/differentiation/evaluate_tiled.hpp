#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "differentiation/tiled_stencil.hpp"






template<class T, class Ext, class Op>
auto evaluate_tiled(const T& in, Ext extent, Op op){

    T out(in.size());

    auto padded = make_padded_extent(extent, op);

    const auto a = make_span(in, padded);
    auto b = make_span(out, padded);

    auto v = make_indices(extent, op);


    std::for_each(
        std::begin(v),
        std::end(v),
        [=](auto idx){
    
            b(idx) = op(a, idx);
        }

    );

    return out;
}
