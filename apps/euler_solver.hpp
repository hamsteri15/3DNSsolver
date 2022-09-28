#pragma once

#include "equation/euler.hpp"
#include "equation/euler_flux.hpp"
#include "equation/boundary_condition.hpp"
#include "differentiation/weno.hpp"
#include "differentiation/upwind.hpp"
#include "differentiation/downwind.hpp"
#include "differentiation/evaluate_tiled.hpp"
#include "time_integration/rk1.hpp"
#include "time_integration/rk3.hpp"



struct EulerSolver1D{

    void take_step(Euler<1>& eq, scalar dt){
        
        volVectorField<1, 3> U(eq.grid(), eq.padding());

        auto eos = eq.eos();
        auto prim = eq.primitive_variables();
        U = primitive_to_conserved(prim, eos);


        auto df = [=](auto f){
            volVectorField<1, 3> Utemp(eq.grid(), eq.padding());
            SplitFlux<Vector<3>, 1> F(eq.grid(), eq.padding());
            Utemp = f;
            mirror_all(Utemp);
            F = make_laxfriedrichs_flux(Utemp, eq.eos(), Vector<1>{1});
            return  d_di(F, Weno_left<0>{}, Weno_right<0>{});


        };
        auto d_dt = Rk3();
        
        U = U - d_dt(U, df, dt);
        
        eq.primitive_variables() = conserved_to_primitive(U, eq.eos());
        
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






