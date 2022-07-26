#include "catch.hpp"
#include <iostream>
#include "convection/convection_flux.hpp"
#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

struct Eos {

    inline auto gamma() { return 1.4; }
};

TEST_CASE("1D convection"){

    SECTION("continuity"){
        /*
        scalarField rho({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        scalarField ret = continuity_flux(rho, U, normal);

        CHECK(ret[0] == 1.0);
        CHECK(ret[1] == 1.0);
        */

    }

    SECTION("momentum"){
        /*
        scalarField rho({1.0, 1.0, 1.0});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        vectorField<1> ret = momentum_flux(rho, p, U, normal);

        //TODO: fixme

        print(ret);


//        CHECK(ret[0] == 1.0);
//        CHECK(ret[1] == 1.0);
        */

    }

    SECTION("energy"){
        /*
        scalarField rho({1.0, 1.0, 1.0});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        scalarField ret = energy_flux(rho, p, U, normal, Eos{});

        //TODO: fixme

        print(ret);

//        CHECK(ret[0] == 1.0);
//        CHECK(ret[1] == 1.0);
        */

    }
    SECTION("convection_flux"){
        /*
        scalarField rho({1.0, 1.0, 1.0});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};


        vectorField<3> flux = convection_flux(rho, p, U, normal, Eos{});

        //TODO: fixme

        print(flux);
        */

    }


}

TEST_CASE("1D lax_friedrichs"){

    SECTION("Shock tube"){

        scalar rho_l = 1.0;
        scalar p_l = 1.0;
        scalar u_l = 0.0;

        scalar rho_r = 0.125;
        scalar p_r = 0.1;
        scalar u_r = 0.0;

        scalarField rho({rho_l, rho_r});
        scalarField p({p_l, p_r});
        vectorField<1> U{Vector<1>{0.0}, Vector<1>{0.0}};
        vectorField<1> normal{Vector<1>{u_l}, Vector<1>{u_r}};

        auto [fl, fr] = lax_friedrichs_flux(rho, p, U, normal, Eos{});


        print(fl);
        print(fr);



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
