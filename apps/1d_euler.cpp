#include <iostream>

#include "euler_solver.hpp"
#include "initial_condition.hpp"


int main(){

     
    EulerSolver1D solver;

    scalar dt = 0.0005;
    scalar T = 0.1;
    auto eq = make_euler_equation<1>(extents<1>{40}, extents<1>{2});
    assign_shocktube<0>(eq);

    scalar time = 0.;
    while (time < T){
        solver.takes_step(eq, dt);
        time += dt;
    }


    print(make_full_span(eq.primitive_variables().rho));

    

    return 0;


}