#pragma once

#include "equation/grid.hpp"
#include "common/extents.hpp"
#include "common/vector_field.hpp"
#include "common/runtime_assert.hpp"
#include "common/loop.hpp"

template <size_t N> struct CartesianGrid : public Grid {

    CartesianGrid() = default;
    
    CartesianGrid(extents<N> ext, Vector<N> p0, Vector<N> p1)
        : m_extents(ext), m_p0(p0), m_p1(p1) {
            runtime_assert(all_positive(p1 - p0), "Point p1 components not larger than p0.");
        }

    static constexpr size_t num_dimensions() { return N; }

    size_t point_count() const {return flat_size(m_extents);}

    extents<N> dimensions() const { return m_extents; }

    Vector<N> p0() const {return m_p0;}
    Vector<N> p1() const {return m_p1;}


private:
    extents<N> m_extents;
    Vector<N>  m_p0;
    Vector<N>  m_p1;
};


template<size_t N>
vectorField<N> points(const CartesianGrid<N>& grid){

    auto L = grid.p1() - grid.p0();
    auto nPoints = grid.dimensions();
    Vector<N> delta;
    for (size_t i = 0; i < N; ++i){
        delta[i] = L[i] / scalar(nPoints.extent(i));
    }

    vectorField<N> ret(grid.point_count());

    auto s = make_span(ret, grid.dimensions());

    for (auto indices : all_indices(s)){

        auto arr = get_array_from_tuple(indices);

        Vector<N> p;
        for (size_t i = 0; i < N; ++i){
            
            p[i] = 0.5*delta[i] + delta[i] * arr[i];
        }
        s(indices) = p;
    }

    return ret;

}