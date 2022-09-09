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

    ConservedVariables(const CartesianGrid<N>& grid, extents<N> padding)
        : rho(grid, padding)
        , rhoE(grid, padding)
        , rhoU(grid, padding) {}

    volScalarField<N> rho;
    volScalarField<N> rhoE;
    volVectorField<N, N> rhoU;

};

template<size_t N, class Range>
auto operator+(const ConservedVariables<N>& lhs, const Range& rhs){

    ConservedVariables<N> ret(lhs);

    for (size_t i = 0; i < rhs.size(); ++i){

        ret.rho[i] += rhs[i][0];
        ret.rhoE[i] += rhs[i][1];

        for (size_t j = 0; j < N; ++j){
            ret.rhoU[i][j] += rhs[i][j+2];
        }
    }

    return ret;
}
template<size_t N, class Range>
auto operator-(const ConservedVariables<N>& lhs, const Range& rhs){

    ConservedVariables<N> ret(lhs);

    for (size_t i = 0; i < size_t(rhs.size()); ++i){

        ret.rho[i] -= rhs[i][0];
        ret.rhoE[i] -= rhs[i][1];

        for (size_t j = 0; j < N; ++j){
            ret.rhoU[i][j] -= rhs[i][j+2];
        }
    }

    return ret;
}



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

    ConservedVariables<N> ret(eq.grid(), eq.padding());
    const auto kin = 0.5 * rho * dot(U,U);

    ret.rho  = rho;
    ret.rhoE = p /(eq.eos().gamma() - 1.0) + kin;
    ret.rhoU = rho * U;

    return ret;

}


template<size_t N>
PrimitiveVariables<N> conserved_to_primitive(const Euler<N>& eq, const ConservedVariables<N>& cons){

    PrimitiveVariables<N> ret(eq.grid(), eq.padding());


    auto rho = cons.rho;
    auto E = cons.rhoE / cons.rho;
    auto U = cons.rhoU / cons.rho;


    auto p = (eq.eos().gamma() - 1.0) * rho * (E - 0.5 * dot(U, U));


    ret.rho = cons.rho;
    ret.p = p;
    ret.U = U;


    return ret;

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




