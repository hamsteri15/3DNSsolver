#include "catch.hpp"

#include "equation/cartesian_grid.hpp"
#include "equation/surface_field.hpp"
#include "equation/euler.hpp"
#include "equation/volumetric_field.hpp"

#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"

#include "test_helpers.hpp"


TEST_CASE("Test CartesianGrid"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(CartesianGrid<2>{});
        REQUIRE_NOTHROW(CartesianGrid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1}));
        REQUIRE_THROWS(CartesianGrid(extents<2>{2,2}, Vector<2>{1, 0}, Vector<2>{0,1}));

    }

    SECTION("points"){

        SECTION("1D"){

            CartesianGrid g(extents<1>{2}, Vector<1>{0}, Vector<1>{1});

            vectorField<1> correct{Vector<1>{0.25}, Vector<1>{0.75}};

            CHECK(points(g) == correct);

        }
        
        SECTION("2D"){

            CartesianGrid g(extents<2>{2,2}, Vector<2>{0, 0}, Vector<2>{1, 1});

            vectorField<2> correct
            {
                Vector<2>{0.25, 0.25}, Vector<2>{0.25, 0.75}, 
                Vector<2>{0.75, 0.25}, Vector<2>{0.75, 0.75}
            };

            CHECK(points(g) == correct);
        }


    }


    SECTION("surface_field_dims"){

        CartesianGrid g(extents<2>{2,2}, Vector<2>{0, 0}, Vector<2>{1, 1});
        CHECK(surface_field_dims(0, g) == extents<2>{3, 2});
        CHECK(surface_field_dims(1, g) == extents<2>{2, 3});

    }

    SECTION("surface_count"){
        CartesianGrid g(extents<2>{2,2}, Vector<2>{0, 0}, Vector<2>{1, 1});
        CHECK(surface_count(g) == 12);
    }



}


TEST_CASE("Test SurfaceField"){

    SECTION("Constructors"){
        CartesianGrid g(extents<2>{2,2}, Vector<2>{0, 0}, Vector<2>{1, 1});

        SurfaceField<int, 2> f1(g);
        SurfaceField<int, 2> f2(g);
        SurfaceField<int, 2> f3(g);

        f3 = f1 + f2;

    }

    SECTION("get/set"){

        CartesianGrid g(extents<2>{2,2}, Vector<2>{0, 0}, Vector<2>{1, 1});

        SurfaceField<scalar, 2> f1(g);

        CHECK(f1.get<0>().size() == 6);
        CHECK(f1.get<1>().size() == 6);

        scalarField f2(6, 1);
        f1.set<0>(f2);

        for (size_t i = 0; i < 6; ++i){
            CHECK(f1[i] == 1.0);
        }


    }


}


TEST_CASE("Test VolumetricField"){

    SECTION("Constructors"){

        CartesianGrid<2> grid(extents<2>{2,3}, Vector<2>{0,0}, Vector<2>{1,1});

        volScalarField<2> f1(grid);
        CHECK(f1.dimensions() == extents<2>{2,3});

        volScalarField<2> f2(grid, extents<2>{1,2});
        CHECK(f2.dimensions() == extents<2>{2,3});
        CHECK(f2.padding() == extents<2>{1,2});

    }

    SECTION("make_full_span"){
        
        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> f(grid, extents<2>{1,1});

        auto s = make_full_span(f);
        s(1,1) = 3;
        s(2,1) = 5;
        std::vector<scalar> correct = 
        {
            0,0,0,0,
            0,3,0,0,
            0,5,0,0,
            0,0,0,0
        };

        CHECK(std::vector<scalar>{f.begin(), f.end()} == correct);


    }

    SECTION("make_internal_span"){
        
        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> f(grid, extents<2>{1,1});

        auto s = make_internal_span(f);
        s(0,0) = 1;
        s(0,1) = 2;
        s(1,0) = 3;
        s(1,1) = 4;
        s(-1,0) = 7;
        std::vector<scalar> correct = 
        {
            0,7,0,0,
            0,1,2,0,
            0,3,4,0,
            0,0,0,0
        };

        CHECK(std::vector<scalar>{f.begin(), f.end()} == correct);

    }

    SECTION("interal_begin/end"){

        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> f(grid, extents<2>{1,1});

        CHECK(internal_begin(f) == std::array<size_t, 2>{1,1});
        CHECK(internal_end(f) == std::array<size_t, 2>{3,3});

    }

    
    SECTION("loop internal"){


        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> f(grid, extents<2>{1,1});

        auto s = make_internal_span(f);

        for (auto i : all_indices(s)){
            auto ii = get_array_from_tuple(i);
            s(ii) = 43;

        }

        std::vector<scalar> correct = 
        {
            0,0,0,0,
            0,43,43,0,
            0,43,43,0,
            0,0,0,0
        };
        CHECK(std::vector<scalar>{f.begin(), f.end()} == correct);


        //print(make_full_span(f));



    }
    


    
    SECTION("tiled differentiation"){

        CartesianGrid<2> grid(extents<2>{2,2}, Vector<2>{0,0}, Vector<2>{1,1});
        volScalarField<2> in(grid, extents<2>{1,1});
        volScalarField<2> out(grid, extents<2>{1,1});


        set_linear<0>(make_full_span(in));
        evaluate_tiled(in, out, d_CD2<0>{});

        
        std::vector<scalar> correct = 
        {
            0,0,0,0,
            0,2,2,0,
            0,2,2,0,
            0,0,0,0
        };
        CHECK(std::vector<scalar>{out.begin(), out.end()} == correct);

    }
    

}


TEST_CASE("Test Euler equation"){

    SECTION("PhysicalFlux"){

        

        SECTION("1D"){

            auto eq = make_euler_equation<1>(extents<1>{3}, extents<1>{0});

            auto& p = eq.primitive_variables();

            p.rho = scalarField({0.5, 1.0, 1.0});
            p.p = scalarField({1.0, 1.0, 1.0});
            p.U = vectorField<1>{Vector<1>{1.0}, Vector<1>{1.0}, Vector<1>{1.0}};


            auto flux = compute_flux(eq, Vector<1>{1});


            auto phi = flux.phi;
            auto phiU = flux.phiU;

            CHECK(phi == scalarField({0.5, 1.0, 1.0}));



            CHECK(phiU == 
                vectorField<1>
                {
                    Vector<1>{1.5},
                    Vector<1>{2.0},
                    Vector<1>{2.0}
                });

            p.rho = scalarField({1.4, 1.4, 1.4});

            auto flux2 = compute_flux(eq, Vector<1>{1});

            CHECK(flux2.phiH[0] == Approx(1.75));        
            CHECK(flux2.phiH[1] == Approx(1.75));        
            CHECK(flux2.phiH[2] == Approx(1.75));        
        
        }

    }

    SECTION("max_eigenvalue"){

        /*
        auto eq = make_euler_equation<1>(extents<1>{3}, extents<1>{0});
        assign_shocktube<0>(eq);
        auto mmax = max_eigenvalue(eq, Vector<1>{});

        CHECK(mmax[0] == 3);
        */


    }

    SECTION("laxfriedrichs_flux"){

        auto eq = make_euler_equation<1>(extents<1>{2}, extents<1>{0});
        assign_shocktube<0>(eq);

        auto [fl, fr] = laxfriedrichs_flux(eq, Vector<1>{1});


        //Continuity

        CHECK(fl.phi[0] == Approx(0.59160798)); //Left side domain
        CHECK(fl.phi[1] == Approx(0.0661437)); //Right side domain

        CHECK(fr.phi[0] == Approx(-0.59160798)); //Left side domain
        CHECK(fr.phi[1] == Approx(-0.0661437));  //Right side domain

        //Energy
        
        CHECK(fl.phiH[0] == Approx(1.47901995)); //Left side domain
        CHECK(fl.phiH[1] == Approx(0.1322875)); //Right side domain

        CHECK(fr.phiH[0] == Approx(-1.47901995)); //Left side domain
        CHECK(fr.phiH[1] == Approx(-0.1322875));  //Right side domain

        //Momentum
        
        CHECK(fl.phiU[0][0] == Approx(0.5)); //Left side domain
        CHECK(fl.phiU[1][0] == Approx(0.05)); //Right side domain

        CHECK(fr.phiU[0][0] == Approx(0.5)); //Left side domain
        CHECK(fr.phiU[1][0] == Approx(0.05));  //Right side domain

    }


}
