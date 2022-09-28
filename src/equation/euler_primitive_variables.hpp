#pragma once

#include "equation/cartesian_grid.hpp"
#include "equation/volumetric_field.hpp"
#include "topaz/include/all.hpp"

template <size_t N> struct PrimitiveVariables {

    PrimitiveVariables(const CartesianGrid<N>& grid, extents<N> padding)
        : rho(grid, padding)
        , p(grid, padding)
        , U(grid, padding) {}

    // TODO: use concepts
    template <class Range_t> PrimitiveVariables& operator=(const Range_t& rng) {
        topaz::copy(rng, *this);
        return *this;
    }

    auto begin() {
        auto tpl = topaz::adl_make_tuple(rho.begin(), p.begin(), U.begin());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto begin() const {
        auto tpl = topaz::adl_make_tuple(rho.begin(), p.begin(), U.begin());
        return topaz::detail::make_zip_iterator(tpl);
    }

    auto end() {
        auto tpl = topaz::adl_make_tuple(rho.end(), p.end(), U.end());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto end() const {
        auto tpl = topaz::adl_make_tuple(rho.end(), p.end(), U.end());
        return topaz::detail::make_zip_iterator(tpl);
    }

    bool operator==(const PrimitiveVariables<N>& rhs) const = default;

    volScalarField<N>    rho;
    volScalarField<N>    p;
    volVectorField<N, N> U;
};

template <size_t N, class Eos>
Vector<N + 2> primitive_to_conserved(scalar rho, scalar p, Vector<N> U, const Eos& eos) {

    auto          kin  = 0.5 * rho * dot(U, U);
    auto          rhoE = p / (eos.gamma() - 1.0) + kin;
    auto          rhoU = rho * U;
    Vector<N + 2> ret{};
    ret[0] = rho;
    ret[1] = rhoE;
    for (size_t i = 0; i < N; ++i) { ret[i + 2] = rhoU[i]; }
    return ret;
}

template <size_t N, class Eos>
auto primitive_to_conserved(const PrimitiveVariables<N>& prim, const Eos& eos) {

    auto op = [eos](auto tpl) {
        auto rho = topaz::get<0>(tpl);
        auto p   = topaz::get<1>(tpl);
        auto U   = topaz::get<2>(tpl);
        return primitive_to_conserved(rho, p, U, eos);
    };
    return topaz::transform(prim, op);
}

template <size_t L, class Eos> auto conserved_to_primitive(Vector<L> cons, const Eos& eos) {

    static constexpr size_t N = L - 2;

    auto rho = cons[0];
    auto E   = cons[1] / rho;

    Vector<N> U{};
    for (size_t i = 0; i < N; ++i) { U[i] = cons[i + 2]; }

    auto p = (eos.gamma() - 1.0) * rho * (E - 0.5 * dot(U, U));

    return topaz::adl_make_tuple(rho, p, U);
}

template <class Range_t, class Eos>
auto conserved_to_primitive(const Range_t& cons, const Eos& eos) {

    auto op = [eos](auto v) { return conserved_to_primitive(v, eos); };

    return topaz::transform(cons, op);
}