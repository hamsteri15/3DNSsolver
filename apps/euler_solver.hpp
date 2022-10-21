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

template<size_t N>
struct EulerSolver{

    template<class Df>
    void solve(Euler<N>& eq, scalar dt, Df df){

        auto eos = eq.eos();
        auto prim = eq.primitive_variables();
        auto U = primitive_to_conserved(prim, eos);
        
        auto d_dt = Rk3();
        auto newU = U - d_dt(U, df, dt);
        eq.primitive_variables() = conserved_to_primitive(newU, eq.eos());

    }


};

struct EulerSolver1D : EulerSolver<1>{

    static constexpr size_t N = 1;

    void take_step(Euler<N>& eq, scalar dt){
        
        auto df = [&](auto f){
            volVectorField<N, N+2> Utemp(eq.grid(), eq.padding());
            Utemp = f;
            mirror_all(Utemp);
            
            SplitFlux<Vector<N+2>, N> F(eq.grid(), eq.padding());
            F = make_laxfriedrichs_flux(Utemp, eq.eos(), eq.grid().unit_normal(CartesianAxis::X));
            
            return  d_di(F, Weno_left<0>{}, Weno_right<0>{});
        };
        this->solve(eq, dt, df);
    }

};    

struct EulerSolver2D : EulerSolver<2>{

    static constexpr size_t N = 2;

    void take_step(Euler<N>& eq, scalar dt){
        
        auto df = [&](auto f){
            volVectorField<N, N+2> Utemp(eq.grid(), eq.padding());
            
            Utemp = f;
            mirror_all(Utemp);
            
            SplitFlux<Vector<N+2>, N> Fx(eq.grid(), eq.padding());
            Fx = make_laxfriedrichs_flux(Utemp, eq.eos(), eq.grid().unit_normal(CartesianAxis::X));
            auto Rx =  d_di(Fx, Weno_left<1>{}, Weno_right<1>{});

            SplitFlux<Vector<N+2>, N> Fy(eq.grid(), eq.padding());
            Fy = make_laxfriedrichs_flux(Utemp, eq.eos(), eq.grid().unit_normal(CartesianAxis::Y));
            auto Ry =  d_di(Fy, Weno_left<0>{}, Weno_right<0>{});

            
            return Rx + Ry;
        };
        this->solve(eq, dt, df);
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






