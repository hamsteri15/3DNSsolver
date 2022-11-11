#pragma once

#include <jada.hpp>
#include "fields/vector_field.hpp"
#include "fields/scalar_field.hpp"
#include "fields/coordinate_system.hpp"


template <size_t N> struct CartesianGrid {

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
    static constexpr size_t axis_index(CartesianAxis ax) {return coordinate_system.axis_index(ax);}


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

template<size_t N> Vector<N> index_to_cell_center(const CartesianGrid<N>& grid, auto indices){

    runtime_assert(indices_in_bounds(indices, grid.dimensions()), "Index out of bounds");

    const auto delta = spatial_stepsize(grid);
    const auto p0 = grid.p0();

    const auto idx = tuple_to_array(indices);


    Vector<N> c{};
    for (size_t i = 0; i < N; ++i){
        c[i] = p0[i] + 0.5 * delta[i] + delta[i] * scalar(idx[i]);
    }
    return c;
    
}




template <size_t N> auto cell_centers(const CartesianGrid<N>& grid) {

    auto delta = spatial_stepsize(grid);
    auto dims  = grid.dimensions();

    std::vector<scalarField> ret(N);
    auto p0 = grid.p0();

    for (size_t i = 0; i < N; ++i) {

        size_t n = dims.extent(i);

        scalarField f(n);

        for (size_t j = 0; j < n; ++j) { f[j] = p0[i] + 0.5 * delta[i] + delta[i] * scalar(j); }
        ret[i] = f;
    }

    return ret;
}

template<size_t N> auto vertex_dimensions(const CartesianGrid<N>& grid){
    auto dims  = extent_to_array(grid.dimensions());
    for (size_t i = 0; i < N; ++i){
        dims[i] += 1;
    }
    return extents<N>{dims};
}

template <size_t N> auto vertices(const CartesianGrid<N>& grid) {

    auto delta = spatial_stepsize(grid);
    auto e_dims  = vertex_dimensions(grid);

    std::vector<scalarField> ret(N);

    auto p0 = grid.p0();

    for (size_t i = 0; i < N; ++i) {

        size_t n = e_dims.extent(i);

        scalarField f(n);

        for (size_t j = 0; j < n; ++j) { f[j] = p0[i] + delta[i] * scalar(j); }
        ret[i] = f;
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
