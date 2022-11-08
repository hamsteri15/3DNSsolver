#pragma once

#include "differentiation/tiled_stencil.hpp"

template <size_t Dir> struct Downwind1 : public TiledStencil<Downwind1<Dir>> {

    static constexpr size_t padding   = 1;
    static constexpr size_t direction = Dir;

    auto operator()(auto f) const {
        return (-f(0) + f(1));
    }
};
