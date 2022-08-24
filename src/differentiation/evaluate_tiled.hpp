#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"
#include "differentiation/tiled_stencil.hpp"



template<size_t N, class ET1, class ET2, class Op>
void evaluate_tiled(span<ET1, N> in, span<ET2, N> out, Op op)
{
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
            out(idx) = op(in, idx);
        }
    );

}
