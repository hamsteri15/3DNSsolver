#include "catch.hpp"

#include "equation/coordinate_system.hpp"
#include "equation/cartesian_grid.hpp"
#include "equation/surface_field.hpp"
#include "equation/euler.hpp"
#include "equation/euler_flux.hpp"
#include "equation/volumetric_field.hpp"
#include "equation/boundary_condition.hpp"
#include "equation/flux.hpp"

#include "differentiation/evaluate_tiled.hpp"
#include "differentiation/cd-n.hpp"
#include "differentiation/weno.hpp"
#include "differentiation/upwind.hpp"
#include "differentiation/downwind.hpp"

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
