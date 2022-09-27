#pragma once

#include "equation/euler.hpp"
#include "differentiation/upwind.hpp"
#include "differentiation/downwind.hpp"
#include "equation/flux.hpp"

enum EulerFluxIdx : size_t
{
    mass = 0,
    ene = 1,
    mom_x = 2,
    mom_y = 3,
    mom_z = 4
};

inline auto enthalpy(const auto& rho, const auto& p, const auto& U, auto eos){

    auto gamma = eos.gamma();
    auto c = sqrt(gamma * p / rho);
    auto H = (c * c) / (gamma - scalar(1)) * 0.5 * dot(U, U);
    return H;
}

template<size_t N, class Eos>
inline Vector<N+2> make_physical_flux(scalar rho, scalar p, Vector<N> U, Vector<N> normal, Eos eos){

    auto H = enthalpy(rho, p, U, eos);
    auto phi = dot(U, normal) * rho;        

    auto cont = phi;
    auto ene = phi * H;
    auto mom = phi * U + p * normal;

    Vector<N+2> ret{};
    ret[EulerFluxIdx::mass] = phi;
    ret[EulerFluxIdx::ene] = phi * H;
    for(size_t i = 0; i < N; ++i){
        ret[EulerFluxIdx::mom_x + i] = mom[i];
    }

    return ret;
}


template<size_t N>
Flux<Vector<N+2>, N> make_physical_flux(const Euler<N>& eq, Vector<N> normal){

    Flux<Vector<N+2>, N> F(eq.grid(), eq.padding());

    auto eos = eq.eos();

    auto op = [eos, normal](auto tpl){

        auto rho = topaz::get<0>(tpl);
        auto p = topaz::get<1>(tpl);
        auto U = topaz::get<2>(tpl);
        
        return make_physical_flux(rho, p, U, normal, eos);
    };

    F.value() = topaz::transform(eq.primitive_variables(), op);
    return F;
}




template<size_t N>
SplitFlux<Vector<N+2>, N> make_laxfriedrichs_flux(const Euler<N>& eq, Vector<N> normal){

    auto eos = eq.eos();

    auto op = [eos, normal](auto tpl){
        auto rho = topaz::get<0>(tpl);
        auto p = topaz::get<1>(tpl);
        auto U = topaz::get<2>(tpl);

        auto q = dot(U, normal);

        auto c = sqrt(eos.gamma() * p / rho);
        auto alpha = std::max(mag(q-c), mag(q+c));


        auto F = make_physical_flux(rho, p, U, normal, eos);
        auto cons = primitive_to_conserved(rho, p, U, eos);

        auto fl = 0.5 * (F + alpha * cons);
        auto fr = 0.5 * (F - alpha * cons);


        return topaz::adl_make_tuple(fl ,fr);
    };

    SplitFlux<Vector<N+2>, N> ret(eq.grid(), eq.padding());
    /*
    auto rng = topaz::transform(eq.primitive_variables(), op);

    topaz::copy(rng, topaz::make_range(ret.begin(), ret.end()));
    */    
    //TODO: call from topaz instead
    std::transform(
        eq.primitive_variables().begin(), eq.primitive_variables().end(), 
        ret.begin(), op
    );
    
    return ret;


}




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


    volVectorField<N, N+2> ret(flux.phi.grid(), flux.phi.padding());

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

template<size_t N, class Scheme>
auto d_di(const ConvectionFlux<N>& F, Scheme scheme){

    auto FF = combine_fields(F);
    auto dF(FF);
    auto delta = spatial_stepsize(F.phi.grid())[Scheme::direction];
    evaluate_tiled(FF, dF, scheme);

    auto ret(dF);
    ret = dF / delta;
    return ret;

}



template<size_t N, class Scheme1, class Scheme2>
auto d_di(const std::pair<ConvectionFlux<N>,ConvectionFlux<N>>& F, Scheme1 scheme1, Scheme2 scheme2){

    static constexpr auto dir = Scheme1::direction;
    static_assert(dir == Scheme2::direction, "Direction mismatch in flux differentiation");

    auto Fl = combine_fields(std::get<0>(F));
    auto Fr = combine_fields(std::get<1>(F));

    auto Fl_int(Fl);
    auto Fr_int(Fr);

    evaluate_tiled(Fl, Fl_int, scheme1);
    evaluate_tiled(Fr, Fr_int, scheme2);

    auto dFl(Fl);
    auto dFr(Fr);


    evaluate_tiled(Fl_int, dFl, Upwind1<dir>{});
    evaluate_tiled(Fr_int, dFr, Downwind1<dir>{});


    auto delta = spatial_stepsize(Fl.grid())[dir];

    auto ret(Fl);
    ret = (dFl + dFr) / delta;
    return ret;

}
