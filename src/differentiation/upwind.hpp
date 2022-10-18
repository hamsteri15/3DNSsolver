#pragma once

#include "differentiation/tiled_stencil.hpp"

template <size_t Dir> struct Upwind1 : public TiledStencil<Upwind1<Dir>> {

    static constexpr size_t padding   = 1;
    static constexpr AxisIndex direction = Dir;

    auto operator()(auto f) const {
        return f(0) - f(-1);
    }
};
