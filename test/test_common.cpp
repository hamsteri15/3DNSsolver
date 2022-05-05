#include "catch.hpp"

#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

TEST_CASE("scalarField"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(scalarField{});

    }

}

void temp(const std::vector<double>& d) {
    (void)d;
}

TEST_CASE("vectorField"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(vectorField<2>{});

        REQUIRE_NOTHROW(vectorField<2>(
            scalarField(10, 1), scalarField(10, 2)
        ));


    }

    SECTION("Access"){

        vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
        auto tpl = a.get_all_chunks();
        auto x = topaz::get<0>(tpl);
        auto y = topaz::get<1>(tpl);
        CHECK(x[0] == 1);
        CHECK(y[0] == 2);
    }


    SECTION("Arithmetic operations")
    {

        SECTION("plus"){
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));
            vectorField<2> c = a + b;
        }

        
        SECTION("dot")
        {
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));

            scalarField s = dot(a, b);
            CHECK(s[0] == 1 * 3 + 2 * 4); 

        }



    }


}