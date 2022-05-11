#include "catch.hpp"
#include <iostream>
#include "convection/convection_flux.hpp"
#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

TEST_CASE("ConvectionFlux1D"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(ConvectionFlux1D{});

    }

    /*
    SECTION("2D") {

        scalarField rho(10, 1.2);
        scalarField p(10, 1.0);
        vectorField<2> U(scalarField(10, 1), scalarField(10, 0));
        vectorField<2> normal(scalarField(10, 1), scalarField(10, 0));



        auto flux = convection_flux(rho, p, U, normal, Eos{});

        for (auto it = flux.zipped_begin(); it != flux.zipped_end(); ++it){

            std::cout << topaz::get<0>(*it);

        }


    }
    */


}
