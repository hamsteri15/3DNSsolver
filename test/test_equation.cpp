#include "catch.hpp"

#include "equation/euler.hpp"
#include "equation/euler_flux.hpp"
#include "equation/euler_primitive_variables.hpp"
#include "equation/boundary_condition.hpp"

#include "spatial_schemes/weno.hpp"
#include "spatial_schemes/cd-n.hpp"

#include "test_helpers.hpp"


TEST_CASE("Test boundary_condition"){

    SECTION("mirror"){
        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> f(grid, extents<2>{1,1});

        f = scalarField
        {
            0,0,0,0,
            0,1,2,0,
            0,3,4,0,
            0,0,0,0
        };

        std::vector<scalar> correct = 
        {
            0,1,2,0,
            1,1,2,2,
            3,3,4,4,
            0,3,4,0
        };

        mirror(f, Vector<2>{1,0});
        mirror(f, Vector<2>{0,1});
        mirror(f, Vector<2>{-1,0});
        mirror(f, Vector<2>{0,-1});
        CHECK(std::vector<scalar>{f.begin(), f.end()} == correct);

    }

}

TEST_CASE("Test Euler equation"){

    SECTION("Constructors"){


    }


    SECTION("max_eigenvalue"){

        /*
        auto eq = make_euler_equation<1>(extents<1>{3}, extents<1>{0});
        assign_shocktube<0>(eq);
        auto mmax = max_eigenvalue(eq, Vector<1>{});

        CHECK(mmax[0] == 3);
        */


    }

    

    SECTION("primitive <-> conservative"){

        SECTION("Test 1"){
            auto eq = make_euler_equation<1>(extents<1>{2}, extents<1>{0});

            assign_shocktube<0>(eq);

            PrimitiveVariables<1> p1 = eq.primitive_variables();
            PrimitiveVariables<1> p2 = eq.primitive_variables();

            auto cons = primitive_to_conserved(p1, eq.eos());
            p2 = conserved_to_primitive(cons, eq.eos());


            CHECK(p1 == p2);

        }
        
    }

    


}

TEST_CASE("Test flux"){

    SECTION("make_physical_flux"){

        auto eq = make_euler_equation<1>(extents<1>{2}, extents<1>{0});

        assign_shocktube<0>(eq);
        Flux<Vector<3>, 1> F(eq.grid(), eq.padding());
        F = make_physical_flux(eq, Vector<1>{1});

        CHECK(F.value()[0] == Vector<3>{0, 0, 1});
        CHECK(F.value()[1] == Vector<3>{0, 0, 0.1});

    }

    SECTION("make_laxfriedrichs_flux"){

        auto eq = make_euler_equation<1>(extents<1>{2}, extents<1>{0});
        assign_shocktube<0>(eq);

        SplitFlux<Vector<3>, 1> F(eq.grid(), eq.padding());

        F = make_laxfriedrichs_flux(eq, Vector<1>{1});

        auto fl = F.left_value();
        auto fr = F.right_value();

        using enum EulerFluxIdx;

        //Continuity

        CHECK(fl[0][mass] == Approx(0.59160798)); //Left side domain
        CHECK(fl[1][mass] == Approx(0.0661437)); //Right side domain

        CHECK(fr[0][mass] == Approx(-0.59160798)); //Left side domain
        CHECK(fr[1][mass] == Approx(-0.0661437));  //Right side domain

        
        //Energy
        
        CHECK(fl[0][ene] == Approx(1.47901995)); //Left side domain
        CHECK(fl[1][ene] == Approx(0.1322875)); //Right side domain

        CHECK(fr[0][ene] == Approx(-1.47901995)); //Left side domain
        CHECK(fr[1][ene] == Approx(-0.1322875));  //Right side domain
        
        //Momentum
        
        CHECK(fl[0][mom_x] == Approx(0.5)); //Left side domain
        CHECK(fl[1][mom_x] == Approx(0.05)); //Right side domain

        CHECK(fr[0][mom_x] == Approx(0.5)); //Left side domain
        CHECK(fr[1][mom_x] == Approx(0.05));  //Right side domain
        
    }

}


TEST_CASE("Test euler_flux"){




    SECTION("Flux differentiation"){

        
        SECTION("unsplit flux"){
            
            auto eq = make_euler_equation<1>(extents<1>{3}, extents<1>{2});
            assign_shocktube<0>(eq);

            Flux<Vector<3>, 1> F(eq.grid(), eq.padding());
            F = make_physical_flux(eq, Vector<1>{1});

            auto dF = d_di<0>(F, d_CD2{});
            
            CHECK(dF[2][mass] == Approx(0));
            CHECK(dF[3][mass] == Approx(0));

            CHECK(dF[2][ene] == Approx(0));
            CHECK(dF[3][ene] == Approx(0));

            CHECK(dF[2][mom_x] == Approx(-1.35));
            CHECK(dF[3][mom_x] == Approx(-1.35));
            
        }
        
        SECTION("split flux"){
            
            auto eq = make_euler_equation<1>(extents<1>{3}, extents<1>{2});
            assign_shocktube<0>(eq);

            SplitFlux<Vector<3>, 1> F(eq.grid(), eq.padding());
            F = make_laxfriedrichs_flux(eq, Vector<1>{1});
            
            auto dF = d_di<0>(F, Weno_left{}, Weno_right{});
            
            using enum EulerFluxIdx;

            CHECK(dF[2][mass] == Approx(1.57639));
            CHECK(dF[3][mass] == Approx(-1.57639));

            CHECK(dF[2][ene] == Approx(4.0402));
            CHECK(dF[3][ene] == Approx(-4.0402));

            CHECK(dF[2][mom_x] == Approx(-1.35));
            CHECK(dF[3][mom_x] == Approx(-1.35));
            
        }

    }
}

