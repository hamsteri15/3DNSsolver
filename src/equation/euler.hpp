#pragma once

#include "equation/cartesian_grid.hpp"
#include "equation/equation_of_state.hpp"
#include "equation/surface_field.hpp"
#include "equation/volumetric_field.hpp"
#include "equation/euler_primitive_variables.hpp"

template <size_t N> struct Euler {

    Euler(const CartesianGrid<N>& grid, extents<N> padding, EquationOfState eos)
        : m_grid(grid)
        , m_variables(grid, padding)
        , m_eos(eos) {}

    auto eos() const { return m_eos; }

    const auto& primitive_variables() const { return m_variables; }
    auto&       primitive_variables() { return m_variables; }

    const auto& grid() const { return m_grid; }
    auto&       grid() { return m_grid; }

    auto padding() const { return primitive_variables().rho.padding(); }

private:
    CartesianGrid<N>      m_grid;
    PrimitiveVariables<N> m_variables;
    EquationOfState       m_eos;
};


template <size_t N, class NormalField>
inline auto max_eigenvalue(const Euler<N>& eq, const NormalField& normal) {

    const auto& prim = eq.primitive_variables();
    const auto& rho  = prim.rho;
    const auto& p    = prim.p;
    const auto& U    = prim.U;

    const auto q = dot(U, normal);
    const auto c = sqrt(eq.eos().gamma() * p / rho);

    const auto eig1 = mag(q - c);
    const auto eig2 = mag(q + c);
    return max(eig1, eig2);
}

template <size_t N> auto max_eigenvalue(const Euler<N>& eq, const Vector<N>& normal) {

    return max_eigenvalue(eq,
                          topaz::make_constant_range(normal, eq.primitive_variables().p.size()));
}
