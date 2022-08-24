#include <iostream>
#include "common/scalar_field.hpp"
#include "common/mdspan.hpp"
#include "common/make_field.hpp"
#include "differentiation/cd-n.hpp"

/*

primitive variables:
W = [rho, p, u] 

conserved variables:
U = [rho, rho*E, rho*u]

*/

auto make_fields(size_t n){


    dd_CD2<0> dd_scheme;
    d_CD2<0> d_scheme;    

    auto rho = make_scalar_field(extents<1>{n}, d_scheme, dd_scheme);
    auto p = make_scalar_field(extents<1>{n}, d_scheme, dd_scheme);
    auto u = make_vector_field<1>(extents<1>{n}, d_scheme, dd_scheme);

    return std::make_tuple(rho,p,u);
}



int main(){

    std::cout << "Solver begins" << std::endl;

    scalar L = 1.0;
    size_t n = 10;
    scalar dx = L / n;

    auto [rho, p, u] = make_fields(n);

    (void) rho;
    (void) p;
    (void) u;
    (void) L;
    (void) dx;

    std::cout << "End" << std::endl;

    return 0;


}