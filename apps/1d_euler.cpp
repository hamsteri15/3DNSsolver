#include <iostream>
#include "equation/euler.hpp"
#include "equation/euler_flux.hpp"
#include "equation/boundary_condition.hpp"
#include "differentiation/weno.hpp"
#include "differentiation/evaluate_tiled.hpp"


struct Eos{
    scalar gamma() const {return 1.4;}
};



int main(){

    /*
    std::cout << "Solver begins" << std::endl;

    scalar T = 0.3;
    scalar L = 1.0;
    size_t nx = 50;
    scalar dx = L / nx;
    scalar dt = 0.0001;
    size_t nt = T / dt;
    auto [rho, p, U] = make_fields(nx);


    for(size_t n = 0; n < nt; ++n){
    
        scalar time = n * dt;

        std::cout << "Time: " << time << std::endl;

        //boundary update here
        take_step(nx, dx, dt, rho, p, U);


    }


    print(p);

    std::cout << "End" << std::endl;
    */
    return 0;


}