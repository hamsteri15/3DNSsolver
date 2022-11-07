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


    scalar dt = 0.0001;
    scalar T = 0.002;
    size_t nx = 50;
    size_t ny = 51;
    auto eq = make_euler_equation<2>(extents<2>{ny, nx}, extents<2>{2, 2});
    assign_shocktube<0>(eq);


    writer.write(eq.grid());

    scalar time = 0.;
    size_t n_checks = 0;
    size_t nt = 0;
    while (time < T){
        std::cout << "Time: " << time << std::endl;
        solver.take_step(eq, dt);
        if ((nt % 10) == 0){
            writer.write(eq.primitive_variables().rho, "rho", n_checks);
            writer.write(eq.primitive_variables().p, "p", n_checks);
            writer.write(eq.primitive_variables().U, "U", n_checks);
            n_checks++;
        }
        time += dt;
        nt++;
    }


    Reader reader(o_path);
    auto n = reader.checkpoint_count();
    std::cout << "Done, wrote: " << std::to_string(n) << " checkpoints.";

    write_xdmf(o_path, "output.xdmf");
    //print(make_internal_span(eq.primitive_variables().rho));

    

    return 0;


}