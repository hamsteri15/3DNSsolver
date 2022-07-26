#pragma once

#include "common/math.hpp"


template <size_t N>
inline auto continuity_flux(const scalarField&    rho,
                            const vectorField<N>& U,
                            const vectorField<N>& normal) {

    const auto q = dot(U, normal);
    return rho * q;
}

template <size_t N>
inline auto momentum_flux(const scalarField&    rho,
                          const scalarField&    p,
                          const vectorField<N>& U,
                          const vectorField<N>& normal) {

    const auto q = dot(U, normal);
    return rho * q * U + p * normal;
}

template <size_t N, class EqState>
inline auto energy_flux(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            const vectorField<N>& normal,
                            EqState               eos) {


    auto q     = dot(U, normal);
    auto gamma = eos.gamma();
    auto c     = sqrt(gamma * p / rho);
    auto H     = sqr(c) / (gamma - scalar(1)) * 0.5 * mag(U*U);

    return rho * q * H;

}
/*
template <size_t N, class EqState>
inline auto convection_flux(const scalarField&    rho,
                            const scalarField&    p,
                            const vectorField<N>& U,
                            const vectorField<N>& normal,
                            EqState               eos) {

    auto q     = dot(U, normal);
    auto gamma = eos.gamma();
    auto c     = sqrt(gamma * p / rho);
    auto H     = sqr(c) / (gamma - scalar(1)) * 0.5 * U * U;




}
*/

/*
inline auto
convection_flux(const auto& rho, const auto& p, const auto& U, const auto& normal, auto eos) {

    auto q     = dot(U, normal);
    auto gamma = eos.gamma();
    auto c     = sqrt(gamma * p / rho);
    auto H     = sqr(c) / (gamma - scalar(1)) * 0.5 * U * U;

    auto cont = rho * q;
    auto ene  = rho * q * H;
    auto mom(rho * q * U + p * normal);

    static constexpr size_t N = 2; //topaz::chunk_count(U);

    vectorField<N + size_t(2)> ret(rho.size());

    ret.set_components(0, cont);
    ret.set_components(1, ene);
    for (size_t i = 0; i < N; ++i){
        ret.set_components(i + 2, topaz::get_chunk(i, mom));
    }
    return ret;

}
*/

/*
void ConvectionFlux::euler_flux_from_primitive(
    const double* W, double* F, double gamma, double normal_x, double normal_y, double normal_z) {

    const double rho = W[0];
    const double p   = W[1];
    const double u   = W[2];
    const double v   = W[3];
    const double w   = W[4];

    const double q_n = u * normal_x + v * normal_y + w * normal_z;          // projected velocity
    const double c = sqrt(gamma * p / rho);                                 // sound speed
    const double H = c * c / (gamma - 1.0) + 0.5 * (u * u + v * v + w * w); // enthalpy

    F[0] = rho * q_n;
    F[1] = rho * q_n * H;
    F[2] = rho * q_n * u + p * normal_x;
    F[3] = rho * q_n * v + p * normal_y;
    F[4] = rho * q_n * w + p * normal_z;
}
*/
