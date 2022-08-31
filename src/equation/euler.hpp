#pragma once

#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"
#include "convection/convection_flux.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/surface_field.hpp"

template <size_t N> struct PrimitiveVariables {

    PrimitiveVariables(size_t size)
        : rho(size)
        , p(size)
        , U(size) {}

    scalarField    rho;
    scalarField    p;
    vectorField<N> U;
};

struct EquationOfState {

    double gamma() const { return 1.4; }
};

template <size_t N> struct Euler {

    Euler(const CartesianGrid<N>& grid, EquationOfState eos)
        : m_grid(grid)
        , m_variables(flat_size(grid.dimensions()))
        , m_eos(eos) {}

    auto eos() const { return m_eos; }

    const auto& primitive_variables() const { return m_variables; }
    auto&       primitive_variables() { return m_variables; }

    const auto& grid() const { return m_grid; }

    auto& grid() { return m_grid; }

private:
    CartesianGrid<N>      m_grid;
    PrimitiveVariables<N> m_variables;
    EquationOfState       m_eos;
};

template <size_t N> inline auto convection_flux(const Euler<N>& eq) {

    SurfaceVectorField<N, N+2> ret(eq.grid());

    const auto& primitive = eq.primitive_variables();

    for (size_t I = 0; I < N; ++I) {


        Vector<N> normal{};
        normal[I] = 1.0;
        vectorField<N> normal_field(primitive.rho.size(), normal);

        auto Fi = convection_flux(primitive.rho, primitive.p, primitive.U, normal_field, eq.eos());

        ret.set(I, Fi);
    }

    return ret;
}
