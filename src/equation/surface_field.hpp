#pragma once

#include "common/field.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/grid.hpp"
#include "topaz/include/all.hpp"
#include <cstddef>

template <class ET, size_t N> struct SurfaceField : public Field<ET> {

private:
    using base_type = Field<ET>;

public:
    using base_type::operator=;

    SurfaceField(const CartesianGrid<N>& grid)
        : Field<ET>(surface_count(grid))
        , m_grid(grid) {}

    template <size_t I> auto get() const {
        return get(I);
    }

    auto get(size_t I) const{
        auto [b_idx, e_idx] = get_chunk(I);
        return topaz::slice(*this, b_idx, e_idx);
    }

    template <size_t I, class Field_t> void set(const Field_t& f) {
        set(I, f);
    }
    
    template <class Field_t> void set(size_t I, const Field_t& f) {
        auto [b_idx, e_idx] = get_chunk(I);

        runtime_assert((e_idx - b_idx) == f.size(), "Size mismatch in SurfaceField::set()");

        auto dst = topaz::slice(*this, b_idx, e_idx);
        topaz::copy(f, dst);
    }

    const auto& get_grid() const { return m_grid; }
    auto&       get_grid() { return m_grid; }

private:
    CartesianGrid<N> m_grid;

    template <size_t I> auto get_chunk() const {

        static_assert(I < N, "Invalid dimension");
        return get_chunk(I);
    }

    auto get_chunk(size_t I) const{

        size_t begin = 0;
        for (size_t i = 0; i < I; ++i) { begin += flat_size(surface_field_dims(i, m_grid)); }
        size_t end = begin + flat_size(surface_field_dims(I, m_grid));

        return std::make_pair(begin, end);

    }

};

template<size_t N>
using SurfaceScalarField = SurfaceField<scalar, N>;

template<size_t N, size_t L>
using SurfaceVectorField = SurfaceField<Vector<L>, N>;