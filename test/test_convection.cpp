#include "catch.hpp"
#include <iostream>
#include "convection/convection_flux.hpp"
#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

TEST_CASE("1D convection"){

    SECTION("continuity"){
        scalarField rho({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        scalarField ret = continuity_flux(rho, U, normal);

        CHECK(ret[0] == 1.0);
        CHECK(ret[1] == 1.0);

    }

    SECTION("momentum"){
        scalarField rho({1.0, 1.0, 1.0});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        vectorField<1> ret = momentum_flux(rho, p, U, normal);

        print(ret);

//        CHECK(ret[0] == 1.0);
//        CHECK(ret[1] == 1.0);

    }


}

TEST_CASE("2D convection"){



}

TEST_CASE("ConvectionFlux1D"){

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
