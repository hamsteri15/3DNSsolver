#pragma once

#include "equation/euler.hpp"


template<size_t N> struct ConvectionFlux{

    ConvectionFlux(const CartesianGrid<N>& grid, extents<N> padding)
        : phi(grid, padding)
        , phiH(grid, padding)
        , phiU(grid, padding) {}


    volScalarField<N> phi;
    volScalarField<N> phiH;
    volVectorField<N, N> phiU;

};


template<size_t N>
volVectorField<N, N+2> combine_fields(const ConvectionFlux<N>& flux){


    volVectorField<N, N+2> ret(flux.phi.get_grid(), flux.phi.padding());

    const auto con = flux.phi;
    const auto ene = flux.phiH;
    const auto mom = flux.phiU;

    auto tuple_begin = topaz::adl_make_tuple(con.begin(), ene.begin(), mom.begin());
    auto tuple_end = topaz::adl_make_tuple(con.end(), ene.end(), mom.end());


    auto op = [](const auto& tpl){
        Vector<N+2> vBig;
        vBig[0] = topaz::get<0>(tpl);
        vBig[1] = topaz::get<1>(tpl);

        Vector<N> v = topaz::get<2>(tpl);

        for (size_t i = 0; i < N; ++i){
            vBig[i + 2] = v[i];
        }
        return vBig;
    };

    ret = topaz::transform(topaz::ZipRange(tuple_begin, tuple_end), op);

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

    ConvectionFlux<N> ret(eq.grid(), eq.padding());

    auto phi = dot(U, normal) * rho;

    ret.phi  = phi;
    ret.phiH = phi * H;
    ret.phiU = phi*U + p * normal;

    return ret;
}

template <size_t N> ConvectionFlux<N> compute_flux(const Euler<N>& eq, const Vector<N>& normal) {

    return compute_flux(eq, topaz::make_constant_range(normal, eq.primitive_variables().p.size()));
}


template<size_t N, class NormalField>
std::pair<ConvectionFlux<N>, ConvectionFlux<N>>
laxfriedrichs_flux(const Euler<N>& eq, const NormalField& normal){

    auto F = compute_flux(eq, normal);
    auto cons = compute_conserved(eq);
    auto alpha = max_eigenvalue(eq, normal);

    ConvectionFlux<N> fl(eq.grid(), eq.padding());
    ConvectionFlux<N> fr(eq.grid(), eq.padding());

    fl.phi = 0.5 * (F.phi + alpha*cons.rho);
    fl.phiH = 0.5 * (F.phiH + alpha * cons.rhoE);
    fl.phiU = 0.5 * (F.phiU + alpha * cons.rhoU);

    fr.phi = 0.5 * (F.phi - alpha*cons.rho);
    fr.phiH = 0.5 * (F.phiH - alpha * cons.rhoE);
    fr.phiU = 0.5 * (F.phiU - alpha * cons.rhoU);


    return std::make_pair(fl, fr);


}