#pragma once

#include "common/extents.hpp"
#include "common/loop.hpp"
#include "common/runtime_assert.hpp"
#include "common/vector_field.hpp"
#include "equation/grid.hpp"
#include "equation/coordinate_system.hpp"

template <size_t N> struct CartesianGrid : public Grid {

    static constexpr Cartesian<N> coordinate_system{};


    CartesianGrid() = default;

    CartesianGrid(extents<N> ext, Vector<N> p0, Vector<N> p1)
        : m_extents(ext)
        , m_p0(p0)
        , m_p1(p1) {
        runtime_assert(all_positive(p1 - p0), "Point p1 components not larger than p0.");
    }

    static constexpr size_t num_dimensions() { return N; }

    size_t point_count() const { return flat_size(m_extents); }

    extents<N> dimensions() const { return m_extents; }

    Vector<N> p0() const { return m_p0; }
    Vector<N> p1() const { return m_p1; }


    static constexpr Vector<N> unit_normal(CartesianAxis ax) {return coordinate_system.unit_normal(ax);}
    static constexpr AxisIndex axis_index(CartesianAxis ax) {return coordinate_system.axis_index(ax);}


private:
    extents<N> m_extents;
    Vector<N>  m_p0;
    Vector<N>  m_p1;
};

template <size_t N> Vector<N> spatial_stepsize(const CartesianGrid<N>& grid) {

    auto      L       = grid.p1() - grid.p0();
    auto      nPoints = grid.dimensions();
    Vector<N> deltas;
    for (size_t i = 0; i < N; ++i) { deltas[i] = L[i] / scalar(nPoints.extent(i)); }
    return deltas;
}

template <size_t N> vectorField<N> points(const CartesianGrid<N>& grid) {

    auto delta = spatial_stepsize(grid);

    vectorField<N> ret(grid.point_count());

    auto s = make_span(ret, grid.dimensions());

    for (auto indices : all_indices(s)) {

        auto arr = get_array_from_tuple(indices);

        Vector<N> p;
        for (size_t i = 0; i < N; ++i) { p[i] = 0.5 * delta[i] + delta[i] * arr[i]; }
        s(arr) = p;
    }

    return ret;
}

template <size_t N> auto surface_field_dims(size_t dir, const CartesianGrid<N>& grid) {

    runtime_assert(dir < N, "Invalid direction.");
    auto d = extent_to_array(grid.dimensions());
    d[dir] += 1;
    return extents<N>{d};
}

template <size_t N> auto surface_count(const CartesianGrid<N>& grid) {

    size_t count = 0;
    for (size_t i = 0; i < N; ++i) { count += flat_size(surface_field_dims(i, grid)); }
    return count;
}
