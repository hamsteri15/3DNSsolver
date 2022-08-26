#pragma once

#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"
#include "equation/cartesian_grid.hpp"

template <size_t N> struct PrimitiveVariables {

    PrimitiveVariables(size_t size)
        : rho(size)
        , p(size)
        , U(size) {}

    scalarField    rho;
    scalarField    p;
    vectorField<N> U;
};





template <size_t N> struct Euler {

    Euler(const CartesianGrid<N>& grid)
        : m_grid(grid)
        , m_variables(flat_size(grid.dimensions())) {}

private:
    CartesianGrid<N>      m_grid;
    PrimitiveVariables<N> m_variables;
};
