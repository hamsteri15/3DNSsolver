#include <iostream>
#include "common/scalar_field.hpp"
#include "common/mdspan.hpp"
#include "common/make_field.hpp"
#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"

/*

primitive variables:
W = [rho, p, u] 

conserved variables:
U = [rho, rho*E, rho*u]

*/


/*
//TODO: something like this
auto W = primitive_variables(grid);

auto F = numerical_flux(primitiveVariables);

auto U = conservative_variables(primitiveVariables);

Equation euler
(
	d_dt(U) + div(F) == zero;
);

euler.solve();

W = conservative_to_primitive(U);

*/

struct Eos{

    scalar gamma() const {return 1.4;}

};

static constexpr auto d_scheme = d_CD2<0>{};

auto make_fields(size_t n){



    auto rho = make_scalar_field(extents<1>{n}, d_scheme);
    auto p = make_scalar_field(extents<1>{n}, d_scheme);
    auto U = make_vector_field<1>(extents<1>{n}, d_scheme);


    for (size_t i = 0; i < rho.size(); ++i){

        if (i < size_t(0.5*rho.size())){

            rho[i] = 1.0;
            p[i] = 1.0;
            U[i] = Vector<1>({0.0});

        }

        else{
            rho[i] = 0.125;
            p[i] = 0.1;
            U[i] = Vector<1>({0.0});

        }

    }


    return std::make_tuple(rho,p,U);
}


void take_step(size_t n, scalar dx, scalar dt, scalarField& rho, scalarField& p, vectorField<1> U){

    extents<1> interior{n};
    auto padded = make_padded_extent(interior, d_scheme);


    vectorField<1> normal(rho.size(), Vector<1>{1});

    vectorField<3> F = convection_flux(rho, p, U, normal, Eos{});
    vectorField<3> dF(F.size());


    evaluate_tiled(
        make_span(F, padded),
        make_span(dF, padded),
        d_scheme
    );

    
    vectorField<3> cons = primitive_to_conservative(rho, p, U, Eos{});
    vectorField<3> dCons = (-dt*dF/dx);


    vectorField<3> cons_new = cons + dCons;
    

    auto [rho_new, p_new, U_new] = conservative_to_primitive(cons_new, Eos{});


    rho = rho_new;
    p = p_new;
    U = U_new;

}


int main(){

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

    return 0;


}