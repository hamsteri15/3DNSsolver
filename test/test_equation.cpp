#include "catch.hpp"

#include "equation/cartesian_grid.hpp"

TEST_CASE("CartesianGrid"){

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



}