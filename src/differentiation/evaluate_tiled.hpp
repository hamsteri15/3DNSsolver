#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "common/subspan.hpp"
#include "differentiation/tiled_stencil.hpp"
#include "equation/volumetric_field.hpp"




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


template<size_t Dir, size_t N, class ET, class Op>
void evaluate_tiled(const VolumetricField<ET, N>& in, VolumetricField<ET, N>& out, Op op){
    
    auto s_in = make_internal_span(in);
    auto s_out = make_internal_span(out);
    evaluate<Dir>
    (
        s_in,
        s_out,
        op,
        all_indices(s_in)
    );

}
