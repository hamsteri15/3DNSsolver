#include <iostream>

#include "euler_solver.hpp"
#include "initial_condition.hpp"
#include "io/write.hpp"
#include "io/make_file.hpp"
#include "io/writer.hpp"

int main(){

     
    EulerSolver2D solver;


    Writer writer("output.h5");


    scalar dt = 0.0001;
    scalar T = 0.3;
    size_t nx = 50;
    size_t ny = 51;
    auto eq = make_euler_equation<2>(extents<2>{ny, nx}, extents<2>{2, 2});
    assign_shocktube<0>(eq);


    writer.write(eq.grid());

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