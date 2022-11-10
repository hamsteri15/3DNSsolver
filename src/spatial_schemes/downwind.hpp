#pragma once

#include "jada/tiled_stencil.hpp"

struct Downwind1 : public TiledStencil<Downwind1> {

    static constexpr size_t padding   = 1;

    auto operator()(auto f) const {
        return (-f(0) + f(1));
    }
};