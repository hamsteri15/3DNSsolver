#include "catch.hpp"
#include <iostream>
#include "convection/convection_flux.hpp"
#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

struct Eos {

    inline auto gamma() { return 1.4; }
};



TEST_CASE("1D tests"){

    SECTION("continuity"){

        scalarField rho({0.5, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        scalarField ret = continuity_flux(rho, U, normal);

        CHECK(ret[0] == 0.5);
        CHECK(ret[1] == 1.0);
        CHECK(ret[2] == 1.0);

    }

    SECTION("momentum"){
        scalarField rho({0.5, 1.0, 1.0});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        vectorField<1> ret = momentum_flux(rho, p, U, normal);

        CHECK(ret[0] == Vector<1>{1.5});
        CHECK(ret[1] == Vector<1>{2.0});
        CHECK(ret[2] == Vector<1>{2.0});


    }

    SECTION("energy"){

        scalarField rho({1.4, 1.4, 1.4});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};

        scalarField ret = energy_flux(rho, p, U, normal, Eos{});

        CHECK(ret[0] == Approx(1.75));
        CHECK(ret[1] == Approx(1.75));
        CHECK(ret[2] == Approx(1.75));


    }
    SECTION("convection_flux"){

        scalarField rho({1.4, 1.4, 1.4});
        scalarField p({1.0, 1.0, 1.0});
        vectorField<1> U{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};
        vectorField<1> normal{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};


        vectorField<3> flux = convection_flux(rho, p, U, normal, Eos{});

        for (auto v : flux){

            REQUIRE(v[0] == Approx(1.4));
            REQUIRE(v[1] == Approx(1.75));
            REQUIRE(v[2] == Approx(2.4));
        }

    }

    SECTION("lax_friedrichs"){

        SECTION("Shock tube"){

            scalar rho_l = 1.0;
            scalar p_l = 1.0;
            scalar u_l = 0.0;

            scalar rho_r = 0.125;
            scalar p_r = 0.1;
            scalar u_r = 0.0;

            scalarField rho({rho_l, rho_r});
            scalarField p({p_l, p_r});
            vectorField<1> U{Vector<1>{u_l}, Vector<1>{u_r}};
            vectorField<1> normal{Vector<1>{1}, Vector<1>{1}};

            auto [fl, fr] = lax_friedrichs_flux(rho, p, U, normal, Eos{});

            //Left-split, on the left side of the domain
            CHECK(fl[0][0] == Approx(0.59160798));
            CHECK(fl[0][1] == Approx(1.47901995));
            CHECK(fl[0][2] == Approx(0.5));

            //Left-split, on the right side of the domain
            CHECK(fl[1][0] == Approx(0.0661437));
            CHECK(fl[1][1] == Approx(0.1322875));
            CHECK(fl[1][2] == Approx(0.05));

            //Right-split, on the left side of the domain
            CHECK(fr[0][0] == Approx(-0.59160798));
            CHECK(fr[0][1] == Approx(-1.47901995));
            CHECK(fr[0][2] == Approx(0.5));

            //Right-split, on the right side of the domain
            CHECK(fr[1][0] == Approx(-0.0661437));
            CHECK(fr[1][1] == Approx(-0.1322875));
            CHECK(fr[1][2] == Approx(0.05));

        }

    }

}
