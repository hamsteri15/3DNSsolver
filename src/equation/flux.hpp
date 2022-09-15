#pragma once

#include "equation/volumetric_field.hpp"

template<class ET, size_t N>
struct Flux{

    Flux() = default;

    Flux(const CartesianGrid<N>& grid, extents<N> padding)
        : m_f(grid, padding) {}


    auto begin() {return m_f.begin();}
    auto end() {return m_f.end();}


    auto& value() {return m_f;}

private:

    VolumetricField<ET, N> m_f;

};

template<class ET, size_t N>
struct SplitFlux{

    SplitFlux() = default;

    SplitFlux(const CartesianGrid<N>& grid, extents<N> padding)
        : m_f_left(grid, padding), m_f_right(grid, padding) {}

    auto begin() {
        auto tpl = topaz::adl_make_tuple(m_f_left.begin(), m_f_right.begin());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto end() {
        auto tpl = topaz::adl_make_tuple(m_f_left.end(), m_f_right.end());
        topaz::detail::make_zip_iterator(tpl);
    }

    VolumetricField<ET, N> m_f_left;
    VolumetricField<ET, N> m_f_right;

};


