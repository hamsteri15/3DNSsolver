#pragma once

#include "jada/mdspan.hpp"
#include "jada/loop.hpp"
#include "jada/subspan.hpp"
#include "jada/tiled_stencil.hpp"





template<size_t Dir, class Span1, class Span2, class Op, class Indices>
void evaluate(Span1 in, Span2 out, Op op, Indices indices){

    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            auto ii = tuple_to_array(idx); //TODO: get rid of this!
            auto stencil = make_tiled_subspan<Dir>(in, ii);
            out(ii) = op(stencil);
        }
    );
}

template<size_t Dir>
void evaluate_tiled(auto in, auto out, auto op)
{
    static_assert(decltype(in)::rank() == decltype(out)::rank(), "Rank mismatch in evaluate tiled.");

    static constexpr auto N = decltype(in)::rank();
    runtime_assert(in.extents() == out.extents(), "Dimension mismatch");

    std::array<size_t, N> begin{};
    std::array<size_t, N> end = extent_to_array(in.extents());

    begin[Dir] += op.padding;
    end[Dir] -= op.padding;

    evaluate<Dir>(in, out, op, md_indices(begin, end));
   
}
