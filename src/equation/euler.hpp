#pragma once

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

template<size_t N> struct ConservedVariables{

    volScalarField<N> rho;
    volScalarField<N> rhoE;
    volVectorField<N, N> rhoU;

};

template<size_t N> struct ConvectionFlux{

    volScalarField<N> phi;
    volScalarField<N> phiH;
    volVectorField<N, N> phiU;

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


template<size_t N>
ConservedVariables<N> compute_conserved(const Euler<N>& eq){

    const auto& prim = eq.primitive_variables();
    const auto& rho = prim.rho;
    const auto& p = prim.p;
    const auto& U = prim.U;

    ConservedVariables<N> ret;
    const auto kin = 0.5 * rho * dot(U,U);

    ret.rho  = rho;
    ret.rhoE = p /(eq.eos().gamma() - 1.0) + kin;
    ret.rhoU = rho * U;

    return ret;

}

inline auto enthalpy(const auto& rho, const auto& p, const auto& U, auto eos){

    auto gamma = eos.gamma();
    auto c = sqrt(gamma * p / rho);
    auto H = (c * c) / (gamma - scalar(1)) * 0.5 * dot(U, U);
    return H;
}

template<size_t N, class NormalField>
ConvectionFlux<N> compute_flux(const Euler<N>& eq, const NormalField& normal){

    const auto& prim = eq.primitive_variables();
    const auto& rho = prim.rho;
    const auto& p = prim.p;
    const auto& U = prim.U;

    auto H = enthalpy(rho, p, U, eq.eos());

    ConvectionFlux<N> ret;

    auto phi = dot(U, normal) * rho;

    ret.phi  = phi;
    ret.phiH = phi * H;
    ret.phiU = phi*U + p * normal;

    return ret;
}

template <size_t N> ConvectionFlux<N> compute_flux(const Euler<N>& eq, const Vector<N>& normal) {

    return compute_flux(eq, topaz::make_constant_range(normal, eq.primitive_variables().p.size()));
}


template <size_t N, class NormalField>
inline auto max_eigenvalue(const Euler<N>& eq, const NormalField& normal){


    const auto& prim = eq.primitive_variables();
    const auto& rho = prim.rho;
    const auto& p = prim.p;
    const auto& U = prim.U;

    const auto q = dot(U, normal);
    const auto c   = sqrt(eq.eos().gamma() * p / rho);

    const auto eig1 = mag(q - c);
    const auto eig2 = mag(q + c);
    return max(eig1, eig2);

}

template <size_t N> 
auto max_eigenvalue(const Euler<N>& eq, const Vector<N>& normal) {

    return max_eigenvalue(eq, topaz::make_constant_range(normal, eq.primitive_variables().p.size()));
}


template<size_t N, class NormalField>
std::pair<ConvectionFlux<N>, ConvectionFlux<N>>
laxfriedrichs_flux(const Euler<N>& eq, const NormalField& normal){

    auto F = compute_flux(eq, normal);
    auto cons = compute_conserved(eq);
    auto alpha = max_eigenvalue(eq, normal);

    ConvectionFlux<N> fl, fr;

    fl.phi = 0.5 * (F.phi + alpha*cons.rho);
    fl.phiH = 0.5 * (F.phiH + alpha * cons.rhoE);
    fl.phiU = 0.5 * (F.phiU + alpha * cons.rhoU);

    fr.phi = 0.5 * (F.phi - alpha*cons.rho);
    fr.phiH = 0.5 * (F.phiH - alpha * cons.rhoE);
    fr.phiU = 0.5 * (F.phiU - alpha * cons.rhoU);


    return std::make_pair(fl, fr);


}


