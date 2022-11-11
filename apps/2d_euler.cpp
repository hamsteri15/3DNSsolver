#include <iostream>

#include "euler_solver.hpp"
#include "initial_condition.hpp"
#include "io/writer.hpp"
#include "io/reader.hpp"
#include "io/write_xdmf.hpp"

int main(){

     
    EulerSolver2D solver;

    std::string o_path = "output.h5";
    Writer writer(o_path);


    scalar dt = 0.001;
    scalar T = 0.3;
    size_t nx = 50;
    size_t ny = 51;
    auto eq = make_euler_equation<2>(jada::extents<2>{ny, nx}, jada::extents<2>{2, 2});
    assign_shocktube<1>(eq);


    writer.write(eq.grid());


    scalar time = 0.;
    writer.write_checkpoint(eq.primitive_variables(), time);
    //size_t n_checks = 0;
    size_t nt = 0;
    while (time < T){
        std::cout << "Time: " << time << std::endl;
        solver.take_step(eq, dt);
        time += dt;
        nt++;
    }

    writer.write_checkpoint(eq.primitive_variables(), time);

    Reader reader(o_path);
    auto n = reader.checkpoint_count();
    std::cout << "Done, wrote: " << std::to_string(n) << " checkpoints.";

    write_xdmf(o_path, "output.xdmf");
    //print(make_internal_span(eq.primitive_variables().rho));

    

    return 0;


}