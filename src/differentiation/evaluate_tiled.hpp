#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "common/subspan.hpp"
#include "differentiation/tiled_stencil.hpp"
#include "equation/volumetric_field.hpp"


template<size_t N, class Span, class Op>
static inline auto make_stencil_indices(Span s, std::array<size_t, N> center, Op op){

    static constexpr size_t dir = get_direction(op);
    //Note! It is ok to access spans out of bounds, so this just makes a 
    //a subspan centered at the input index which can be called like
    // f(-1), f(0), f(+1) etc.
    return make_tiled_subspan<dir>(
        s,
        center,
        center
    );

}


template<class Span1, class Span2, class Op, class Indices>
void evaluate(Span1 in, Span2 out, Op op, Indices indices){

    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            auto ii = tuple_to_array(idx); 
            auto stencil = make_stencil_indices(in, ii, op);
            out(ii) = op(stencil);
        }
    );
}


template<class Span1, class Span2, class Op>
void evaluate_tiled(Span1 in, Span2 out, Op op)
{
    static_assert(Span1::rank() == Span2::rank(), "Rank mismatch in evaluate tiled.");

    static constexpr auto N = Span1::rank();

    runtime_assert(in.extents() == out.extents(), "Dimension mismatch");
    auto padding = get_padding<N>(op);

    std::array<size_t, N> begin = padding;
    std::array<size_t, N> end{};

    for (size_t i = 0; i < N; ++i){
        end[i] = in.extent(i) - padding[i];
    }

    auto indices = md_indices(begin, end);

    evaluate(in, out, op, indices);
   
}

template<size_t N, class ET, class Op>
void evaluate_tiled(const VolumetricField<ET, N>& in, VolumetricField<ET, N>& out, Op op){
    
    auto s_in = make_internal_span(in);
    auto s_out = make_internal_span(out);
    evaluate
    (
        s_in,
        s_out,
        op,
        all_indices(s_in)
    );

}
