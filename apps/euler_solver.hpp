#pragma once

#include "equation/euler.hpp"
#include "equation/euler_flux.hpp"
#include "equation/boundary_condition.hpp"
#include "differentiation/weno.hpp"
#include "differentiation/upwind.hpp"
#include "differentiation/downwind.hpp"
#include "differentiation/evaluate_tiled.hpp"



template<size_t N>
void mirror_all(Euler<N>& eq){

    auto& p = eq.primitive_variables();

    for (size_t i = 0; i < N; ++i){

        Vector<N> normal{};
        normal[i] = 1;
        mirror(p.rho, normal);
        mirror(p.p, normal);
        mirror(p.U, normal);

        normal[i] = -1;
        mirror(p.rho, normal);
        mirror(p.p, normal);
        mirror(p.U, normal);

    }

}

template<size_t I, size_t N>
auto sweep(const Euler<N>& eq){

    Vector<N> normal{};
    normal[I] = 1.0;
    SplitFlux<Vector<N+2>, N> F(eq.grid(), eq.padding());
    F = make_laxfriedrichs_flux(eq, normal);
    return  d_di(F, Weno_left<I>{}, Weno_right<I>{});
} 

struct EulerSolver1D{



    void takes_step(Euler<1>& eq, scalar dt){

        mirror_all(eq);

        auto U = primitive_to_conserved(eq.primitive_variables(), eq.eos());
        auto dU = sweep<0>(eq);
        auto new_U = U - dt*dU;

        eq.primitive_variables() = conserved_to_primitive(new_U, eq.eos());

    }

};



template<size_t N>
auto make_euler_equation(extents<N> dims, extents<N> padding){

    Vector<N> p0;
    Vector<N> p1;
    p0 = 0.0;
    p1 = 1.0;
    CartesianGrid<N> grid(dims, p0, p1);
    return Euler(grid, padding, EquationOfState{});
}






