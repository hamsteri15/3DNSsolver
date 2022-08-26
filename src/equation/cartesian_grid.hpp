#pragma once

#include "common/extents.hpp"
#include "equation/grid.hpp"

template <size_t N> struct CartesianGrid : public Grid {

    CartesianGrid() = default;
    
    CartesianGrid(extents<N> ext, Vector<N> p0, Vector<N> p1)
        : m_extents(ext), m_p0(p0), m_p1(p1) {}

    static constexpr size_t num_dimensions() { return N; }

    extents<N> dimensions() const { return m_extents; }

private:
    extents<N> m_extents;
    Vector<N>  m_p0;
    Vector<N>  m_p1;
};
