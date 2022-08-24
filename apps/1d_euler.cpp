#include <iostream>
#include "common/scalar_field.hpp"
#include "common/mdspan.hpp"
#include "common/make_field.hpp"
#include "differentiation/cd-n.hpp"
#include "convection/convection_flux.hpp"
#include "differentiation/evaluate_tiled.hpp"

/*

primitive variables:
W = [rho, p, u] 

conserved variables:
U = [rho, rho*E, rho*u]

*/


struct Eos{

    scalar gamma() const {return 1.4;}

};

static constexpr auto d_scheme = d_CD2<0>{};

auto make_fields(size_t n){



    auto rho = make_scalar_field(extents<1>{n}, d_scheme);
    auto p = make_scalar_field(extents<1>{n}, d_scheme);
    auto u = make_vector_field<1>(extents<1>{n}, d_scheme);

    return std::make_tuple(rho,p,u);
}


void take_step(size_t n, scalarField& rho, scalarField& p, vectorField<1> U){

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

    



    (void) F;
    (void) dF;

}


int main(){

    std::cout << "Solver begins" << std::endl;

    scalar L = 1.0;
    size_t n = 10;
    scalar dx = L / n;

    auto [rho, p, U] = make_fields(n);

    take_step(n, rho, p, U);

    (void) rho;
    (void) p;
    (void) U;
    (void) L;
    (void) dx;

    std::cout << "End" << std::endl;

    return 0;


}