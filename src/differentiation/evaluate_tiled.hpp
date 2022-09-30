#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "differentiation/tiled_stencil.hpp"
#include "equation/volumetric_field.hpp"


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

    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            auto ii = get_array_from_tuple(idx); 
            out(ii) = op(in, ii);
        }
    );

}

template<size_t N, class ET, class Op>
void evaluate_tiled(const VolumetricField<ET, N>& in, VolumetricField<ET, N>& out, Op op){
    
    auto s_in = make_internal_span(in);
    auto s_out = make_internal_span(out);
    auto indices = all_indices(s_in);

    std::for_each(
        std::begin(indices),
        std::end(indices),
        [=](auto idx){
            //TODO: This is bullshit but subspans currently dont support ranges:common_tuple indexing
            auto ii = get_array_from_tuple(idx); 
            s_out(ii) = op(s_in, ii);
        }
    );


}

