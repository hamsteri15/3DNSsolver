#pragma once

#include <jada.hpp>

struct Upwind1 : public jada::TiledStencil<Upwind1> {

    static constexpr size_t padding   = 1;

    auto operator()(auto f) const {
        return f(0) - f(-1);
    }
};
