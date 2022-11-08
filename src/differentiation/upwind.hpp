#pragma once

#include "differentiation/tiled_stencil.hpp"

struct Upwind1 : public TiledStencil<Upwind1> {

    static constexpr size_t padding   = 1;

    auto operator()(auto f) const {
        return f(0) - f(-1);
    }
};
