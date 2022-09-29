#include <iostream>

#include "euler_solver.hpp"
#include "initial_condition.hpp"


int main(){

     
    EulerSolver2D solver;

    scalar dt = 0.0001;
    scalar T = 0.3;
    auto eq = make_euler_equation<2>(extents<2>{5, 6}, extents<2>{2, 2});
    assign_shocktube<0>(eq);

    scalar time = 0.;
    while (time < T){
        std::cout << "Time: " << time << std::endl;
        solver.take_step(eq, dt);
        time += dt;
    }

    std::cout << "Done" << std::endl;

    print(make_internal_span(eq.primitive_variables().rho));

    

    return 0;


}