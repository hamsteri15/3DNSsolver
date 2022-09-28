#include <iostream>

#include "euler_solver.hpp"
#include "initial_condition.hpp"


int main(){

     
    EulerSolver2D solver;

    scalar dt = 0.0005;
    scalar T = 0.3;
    auto eq = make_euler_equation<2>(extents<2>{6,5}, extents<2>{2, 2});
    assign_shocktube<0>(eq);

    scalar time = 0.;
    while (time < T){
        solver.take_step(eq, dt);
        time += dt;
    }


    print(make_internal_span(eq.primitive_variables().rho));

    

    return 0;


}