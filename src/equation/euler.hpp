#pragma once

#include "convection/convection_flux.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/surface_field.hpp"
#include "equation/equation_of_state.hpp"
#include "equation/volumetric_field.hpp"


template <size_t N> struct PrimitiveVariables {

    PrimitiveVariables(const CartesianGrid<N>& grid, extents<N> padding)
        : rho(grid, padding)
        , p(grid, padding)
        , U(grid, padding) {}

    volScalarField<N>    rho;
    volScalarField<N>    p;
    volVectorField<N, N> U;
};



template <size_t N> struct Euler {

    Euler(const CartesianGrid<N>& grid, extents<N> padding, EquationOfState eos)
        : m_grid(grid)
        , m_variables(grid, padding)
        , m_eos(eos) {}

    auto eos() const { return m_eos; }

    const auto& primitive_variables() const { return m_variables; }
    auto&       primitive_variables() { return m_variables; }

    const auto& grid() const { return m_grid; }
    auto& grid() { return m_grid; }


    auto padding() const {return primitive_variables().rho.padding();}

private:
    CartesianGrid<N>      m_grid;
    PrimitiveVariables<N> m_variables;
    EquationOfState       m_eos;
};

/*
//W = [rho, p, u]
//U = [rho, rho*E, rho*u]
template<size_t N>
auto compute_conserved(const Euler<N>& eq){

    const auto& prim = eq.primitive_variables();
    const auto& rho = prim.rho;
    const auto& p = prim.p;
    const auto& U = prim.U;

    const auto kin = 0.5 * rho * dot(U,U);
    const auto rhoE = p /(eq.eos().gamma() - 1.0) + kin;
    const auto rhoU = rho * U;

    return std::make_tuple(rho, rhoE, rhoU);

}

template<size_t N>
auto compute_convection(const Euler<N>& eq, Vector<N> normal){

    const auto& prim = eq.primitive_variables();
    const auto& rho = prim.rho;
    const auto& p = prim.p;
    const auto& U = prim.U;

    auto nf = topaz::make_constant_range(normal, p.size());

    auto H = enthalpy(rho, p, U, eq.eos());

    auto phi = dot(U, nf) * rho;
    auto phiH = phi * H;
    auto phiU = phi*U + p * nf;

    return std::make_tuple(phi, phiH, phiU);

}
*/

template<size_t N>
struct ConvectionFlux{

    ConvectionFlux(const Euler<N>& eq)
    {
        
        for (size_t i = 0; i < N; ++i){
            fluxes[i] = volVectorField<N, N+2>(eq.grid(), eq.padding());
        }
        compute_euler_convection(eq);

    } 

    std::array<volVectorField<N, N+2>, N> fluxes;

private:

    void compute_euler_convection(const Euler<N>& eq){

        const auto& p = eq.primitive_variables();        
        for (size_t i = 0; i < N; ++i){
            Vector<N> normal{};
            normal[i] = scalar(1);
            vectorField<N> nf(p.rho.size(), normal);
            auto F = convection_flux(p.rho, p.p, p.U, nf, eq.eos());

            topaz::copy(F, fluxes[i]);
        }        

    }

};



