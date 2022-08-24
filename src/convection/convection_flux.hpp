#pragma once

#include "common/math.hpp"

inline auto continuity_flux(const auto& rho, const auto& U, const auto& normal) {

    const auto q = dot(U, normal);
    return rho * q;
}

inline auto momentum_flux(const auto& rho, const auto& p, const auto& U, const auto& normal) {

    const auto q = dot(U, normal);
    return rho * q * U + p * normal;
}

inline auto
energy_flux(const auto& rho, const auto& p, const auto& U, const auto& normal, auto eos) {

    auto q     = dot(U, normal);
    auto gamma = eos.gamma();
    auto c     = sqrt(gamma * p / rho);
    auto H     = (c * c) / (gamma - scalar(1)) * 0.5 * dot(U, U);

    return rho * q * H;
}


template <size_t N, class EqState>
inline auto convection_flux(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            const vectorField<N>& normal,
                            EqState               eos) {


    const auto con = continuity_flux(rho, U, normal);
    const auto ene = energy_flux(rho, p, U, normal, eos);
    const auto mom = momentum_flux(rho, p, U, normal);

    auto tuple_begin = topaz::adl_make_tuple(con.begin(), ene.begin(), mom.begin());
    auto tuple_end = topaz::adl_make_tuple(con.end(), ene.end(), mom.end());


    auto op = [](const auto& tpl){
        Vector<N+2> ret;
        ret[0] = topaz::get<0>(tpl);
        ret[1] = topaz::get<1>(tpl);

        Vector<N> v = topaz::get<2>(tpl);

        for (size_t i = 0; i < N; ++i){
            ret[i + 2] = v[i];
        }
        return ret;
    };

    auto rng = topaz::transform(topaz::ZipRange(tuple_begin, tuple_end), op);

    return vectorField<N + 2>(rng);

}


template <size_t N, class EqState>
inline auto max_eigenvalue(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            const vectorField<N>& normal,
                            EqState               eos) {

    const auto q = dot(U, normal);
    const auto c   = sqrt(eos.gamma() * p / rho);

    const auto eig1 = mag(q - c);
    const auto eig2 = mag(q + c);
    return max(eig1, eig2);

}
//W = [rho, p, u]
//U = [rho, rho*E, rho*u]
template <size_t N, class EqState>
inline auto primitive_to_conservative(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            EqState               eos) {


    const auto kin = 0.5 * rho * dot(U,U);
    const auto rhoE = p /(eos.gamma() - 1.0) + kin;
    const auto rhoU = rho * U;

    auto begin = topaz::adl_make_tuple(rho.begin(), rhoE.begin(), rhoU.begin());
    auto end = topaz::adl_make_tuple(rho.end(), rhoE.end(), rhoU.end());

    auto op = [](const auto& tpl){
        Vector<N + 2> ret;
        ret[0] = topaz::get<0>(tpl);
        ret[1] = topaz::get<1>(tpl);

        Vector<N> v = topaz::get<2>(tpl);
        for (size_t i = 0; i < N; ++i){
            ret[i + 2] = v[i];
        }
        return ret;
    };

    auto rng = topaz::transform(topaz::ZipRange(begin, end), op);

    return vectorField<N + 2>(rng);

}

template<size_t L, class EqState>
inline auto conservative_to_primitive(const vectorField<L>& cons, EqState eos)
{

    static constexpr size_t N = L - 2;

    scalarField rho_field(cons.size());
    scalarField p_field(cons.size());
    vectorField<N> U_field(cons.size());

    for (size_t j = 0; j < cons.size(); ++j){

        auto v = cons[j];

        scalar rho = v[0];
        scalar E = v[1] / rho;
        
        Vector<N> U;

        for (size_t i = 0; i < N; ++i){
            auto component = v[i + 2];
            U[i] = component / rho;
        }
        scalar p = (eos.gamma() - 1.0) * rho * (E - 0.5 * dot(U,U));
    
        rho_field[j] = rho;
        p_field[j] = p;
        U_field[j] = U;
    }

    return std::make_tuple(rho_field, p_field, U_field);


}


template <size_t N, class EqState>
inline auto lax_friedrichs_flux(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            const vectorField<N>& normal,
                            EqState               eos) {

    const auto F = convection_flux(rho, p, U, normal, eos);
    const auto alpha = max_eigenvalue(rho, p, U, normal, eos);
    const auto cons = primitive_to_conservative(rho, p, U, eos);
    const vectorField<N+2> fl = 0.5*(F + alpha*cons);
    const vectorField<N+2> fr = 0.5*(F - alpha*cons);

    return std::make_tuple(fl, fr);

}
