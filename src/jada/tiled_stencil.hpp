#pragma once

#include <cstddef>

template<class Derived>
struct TiledStencil{
    static constexpr size_t padding = Derived::padding;
};


