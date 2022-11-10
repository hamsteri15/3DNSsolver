#pragma once

#include "jada/tiled_stencil.hpp"


struct d_CD2 : public TiledStencil<d_CD2> {

    static constexpr size_t padding   = 1;

    auto operator()(auto f) const {
        return 0.5*(f(1) - f(-1));
    }
};

struct dd_CD2 : public TiledStencil<dd_CD2>{


    static constexpr size_t padding = 1;

    auto operator()(auto f) const{
        return f(1) - 2.0 * f(0) + f(-1);
    }


};