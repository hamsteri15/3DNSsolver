#pragma once

#include <array>
#include <cstddef>
#include "common/mdspan.hpp"
#include "common/loop.hpp"

template<class Derived>
struct TiledStencil{
    static constexpr size_t padding = Derived::padding;
};


