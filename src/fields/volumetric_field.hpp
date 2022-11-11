#pragma once

#include <jada.hpp>
#include "fields/field.hpp"
#include "fields/cartesian_grid.hpp"

template <class ET, size_t N> struct VolumetricField : public Field<ET> {

    using base_type = Field<ET>;
    using base_type::operator=;

    VolumetricField() = default;

    VolumetricField(const CartesianGrid<N>& grid, jada::extents<N> padding = jada::extents<N>{})
        : Field<ET>(size_to_allocate(grid, padding))
        , m_grid(grid)
        , m_padding(padding) {}

    auto padding() const { return m_padding; }
    auto dimensions() const { return m_grid.dimensions(); }

    const auto& grid() const { return m_grid; }
    auto&       grid() { return m_grid; }

private:
    static size_t size_to_allocate(const CartesianGrid<N>& grid, jada::extents<N> padding) {

        MathVector<size_t, N> full;
        for (size_t i = 0; i < N; ++i) {
            full[i] = grid.dimensions().extent(i) + 2 * padding.extent(i);
        }
        return full.elementwise_product();
    }

    CartesianGrid<N> m_grid;
    jada::extents<N>       m_padding;
};

template<class ET, size_t N>
std::array<size_t, N> full_begin(const VolumetricField<ET, N>& f){
    (void) f;
    return std::array<size_t, N>{};
}


template<class ET, size_t N>
std::array<size_t, N> internal_begin(const VolumetricField<ET, N>& f){

    std::array<size_t, N> begin{};
    for (size_t i = 0; i < N; ++i){
        begin[i] = f.padding().extent(i);
    }
    return begin;
}

template<class ET, size_t N>
std::array<size_t, N> internal_end(const VolumetricField<ET, N>& f){

    auto end = internal_begin(f);
    for (size_t i = 0; i < N; ++i){
        end[i] += f.dimensions().extent(i);
    }
    return end;
}

template<class ET, size_t N>
std::array<size_t, N> full_end(const VolumetricField<ET, N>& f){

    auto end = internal_end(f);
    for (size_t i = 0; i < N; ++i){
        end[i] += f.padding().extent(i);
    }
    return end;
}

template<class ET, size_t N>
jada::extents<N> full_extent(const VolumetricField<ET, N>& f){

    return full_end(f);
}


template<class ET, size_t N>
auto make_full_span(VolumetricField<ET, N>& f){
    return jada::make_span(f, full_extent(f));
}

template<class ET, size_t N>
auto make_full_span(const VolumetricField<ET, N>& f){
    return jada::make_span(f, full_extent(f));
}


template<class ET, size_t N>
auto make_internal_span(VolumetricField<ET, N>& f){
    return jada::make_subspan(make_full_span(f), internal_begin(f), internal_end(f));
}

template<class ET, size_t N>
auto make_internal_span(const VolumetricField<ET, N>& f){
    return jada::make_subspan(make_full_span(f), internal_begin(f), internal_end(f));
}




template <class ET, size_t N>
std::array<size_t, N> boundary_subspan_dims(const VolumetricField<ET, N>& f, Vector<N> normal) {

    std::array<size_t, N> s_dims{};


    for (size_t i = 0; i < N; ++i) {
        if (normal[i] != 0) {
            s_dims[i] = f.padding().extent(i);
        } else {
            s_dims[i] = f.dimensions().extent(i);
        }
    }

    return s_dims;
}

template <class ET, size_t N>
std::array<size_t, N> boundary_subspan_begin(const VolumetricField<ET, N>& f, Vector<N> normal) {

    auto begin = jada::extent_to_array(f.padding());
    for (size_t i = 0; i < N; ++i) {
        if (normal[i] > 0) { begin[i] = f.dimensions().extent(i); }
    }

    return begin;
}
template <class ET, size_t N>
std::array<size_t, N> boundary_subspan_end(const VolumetricField<ET, N>& f, Vector<N> normal) {

    auto begin = boundary_subspan_begin(f, normal);
    auto dims  = boundary_subspan_dims(f, normal);

    std::array<size_t, N> end{};
    for (size_t i = 0; i < N; ++i) { end[i] = begin[i] + dims[i]; }

    return end;
}

template <class ET, size_t N> auto boundary_subspan(VolumetricField<ET, N>& f, Vector<N> normal) {

    return jada::make_subspan(
        make_full_span(f), boundary_subspan_begin(f, normal), boundary_subspan_end(f, normal));
}

template <class ET, size_t N> auto boundary_subspan(const VolumetricField<ET, N>& f, Vector<N> normal) {

    return jada::make_subspan(
        make_full_span(f), boundary_subspan_begin(f, normal), boundary_subspan_end(f, normal));
}


template<class ET, size_t N, class Op>
void for_all_coordinates(VolumetricField<ET, N>& f, Op op){

    auto span = make_internal_span(f);

    for (auto idx : jada::all_indices(span)){
        span(jada::tuple_to_array(idx)) = op(index_to_cell_center(f.grid(), idx));
    }

}


template<size_t Dir, size_t N, class ET, class Op>
void evaluate_tiled(const VolumetricField<ET, N>& in, VolumetricField<ET, N>& out, Op op){
    
    auto s_in = make_internal_span(in);
    auto s_out = make_internal_span(out);
    evaluate<Dir>
    (
        s_in,
        s_out,
        op,
        jada::all_indices(s_in)
    );

}

template<size_t N>
using volScalarField = VolumetricField<scalar, N>;


template<size_t N, size_t L>
using volVectorField = VolumetricField<Vector<L>, N>;


