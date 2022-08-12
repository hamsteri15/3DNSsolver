#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "differentiation/tiled_stencil.hpp"



//Assumes that in and out are already padded and extent represents the physical extent
template<class In, class Out, class Ext, class Op>
void evaluate_tiled(In in, Out out, Ext extent, Op op)
{
    //TODO: this should not take extent as an input parameter anymore,
    //      it should assume that everything is padded and compute op
    //      for all possible indices

    auto indices = make_indices(extent, op);

    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            out(idx) = op(in, idx);
        }

    );
}

/*
TODO: replace above with something like this
template<class In, class Out, class Op>
void evaluate_tiled2(In in, Out out, Op op)
{


    auto padded_extent = in.extents()
    auto interior_extent = make_interior_indices(padded_extent, op);

    auto begin = begin(interior_extent);
    auto end = end(interior_extent);

    auto indices = md_indices(begin, end);


    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            out(idx) = op(in, idx);
        }

    );
}
*/


template<class T, class Ext, class Op>
auto evaluate_tiled(const T& in, Ext extent, Op op){

    //Assumes that in is already padded so the following steps should be somewhere else

    T out(in.size());

    auto padded = make_padded_extent(extent, op);

    const auto a = make_span(in, padded);
    auto b = make_span(out, padded);

    evaluate_tiled(a, b, extent, op);


    return out;
}
