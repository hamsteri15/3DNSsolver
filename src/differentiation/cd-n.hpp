#pragma once

#include "common/mdspan.hpp"
#include "differentiation/tiled_stencil.hpp"


template <size_t Dir> struct d_CD2 : public TiledStencil<d_CD2<Dir>> {

    static constexpr size_t padding   = 1;
    static constexpr size_t direction = Dir;

    auto operator()(auto span, auto idx) const {
        return (span(shift<Dir, 1>(idx)) - span(shift<Dir, -1>(idx)));
    }
};

template<size_t Dir>
struct dd_CD2 : public TiledStencil<dd_CD2<Dir>>{


    static constexpr size_t padding = 1;
    static constexpr size_t direction = Dir;

    auto operator()(auto span, auto idx) const{
        return
        span(shift<Dir, 1>(idx))
        -2.0*span(idx)
        +span(shift<Dir, -1>(idx));
    }

};