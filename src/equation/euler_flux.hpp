#pragma once

#include <variant>

#include "differentiation/downwind.hpp"
#include "differentiation/upwind.hpp"
#include "equation/euler.hpp"
#include "equation/flux.hpp"

enum EulerFluxIdx : size_t { mass = 0, ene = 1, mom_x = 2, mom_y = 3, mom_z = 4 };

inline auto enthalpy(const auto& rho, const auto& p, const auto& U, auto eos) {

    auto gamma = eos.gamma();
    auto c     = sqrt(gamma * p / rho);
    auto H     = (c * c) / (gamma - scalar(1)) * 0.5 * dot(U, U);
    return H;
}

template <size_t N, class Eos>
inline Vector<N + 2>
make_physical_flux(scalar rho, scalar p, Vector<N> U, Vector<N> normal, Eos eos) {

    auto H   = enthalpy(rho, p, U, eos);
    auto phi = dot(U, normal) * rho;

    auto mom = phi * U + p * normal;

    Vector<N + 2> ret{};
    ret[EulerFluxIdx::mass] = phi;
    ret[EulerFluxIdx::ene]  = phi * H;
    for (size_t i = 0; i < N; ++i) { ret[EulerFluxIdx::mom_x + i] = mom[i]; }

    return ret;
}

template <size_t N> auto make_physical_flux(const Euler<N>& eq, Vector<N> normal) {

    Flux<Vector<N + 2>, N> F(eq.grid(), eq.padding());

    auto eos = eq.eos();

    auto op = [eos, normal](auto tpl) {
        auto rho = topaz::get<0>(tpl);
        auto p   = topaz::get<1>(tpl);
        auto U   = topaz::get<2>(tpl);

        return make_physical_flux(rho, p, U, normal, eos);
    };

    return topaz::transform(eq.primitive_variables(), op);
}

template <size_t N> auto make_laxfriedrichs_flux(const Euler<N>& eq, Vector<N> normal) {

    auto eos = eq.eos();

    auto op = [eos, normal](auto tpl) {
        auto rho = topaz::get<0>(tpl);
        auto p   = topaz::get<1>(tpl);
        auto U   = topaz::get<2>(tpl);

        auto q = dot(U, normal);

        auto c     = sqrt(eos.gamma() * p / rho);
        auto alpha = std::max(mag(q - c), mag(q + c));

        auto F    = make_physical_flux(rho, p, U, normal, eos);
        auto cons = primitive_to_conserved(rho, p, U, eos);

        auto fl = 0.5 * (F + alpha * cons);
        auto fr = 0.5 * (F - alpha * cons);

        return topaz::adl_make_tuple(fl, fr);
    };

    return topaz::transform(eq.primitive_variables(), op);
}

template <class VectorField, class Eos, size_t N>auto make_laxfriedrichs_flux(const VectorField& cons, Eos eos, Vector<N> normal) {


    auto op = [eos, normal](auto v) {

        auto tpl = conserved_to_primitive(v, eos);

        auto rho = topaz::get<0>(tpl);
        auto p = topaz::get<1>(tpl);
        auto U = topaz::get<2>(tpl);

        auto q = dot(U, normal);

        auto c     = sqrt(eos.gamma() * p / rho);
        auto alpha = std::max(mag(q - c), mag(q + c));

        auto F    = make_physical_flux(rho, p, U, normal, eos);

        auto fl = 0.5 * (F + alpha * v);
        auto fr = 0.5 * (F - alpha * v);

        return topaz::adl_make_tuple(fl, fr);
    };

    return topaz::transform(cons, op);
}
