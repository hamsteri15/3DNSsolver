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

        REQUIRE_THROWS(vectorField<2>(
            scalarField(10, 1), scalarField(12, 2)
        ));


    }

    SECTION("Access"){
        //using namespace std;



        SECTION("get_all_components const"){


            const vectorField<2> a(scalarField(5, 1), scalarField(5, 2));
            auto tpl = a.get_all_components();
            auto x = std::get<0>(tpl);
            auto y = std::get<1>(tpl);
            CHECK(x[0] == 1);
            CHECK(y[0] == 2);
            CHECK(std::tuple_size<decltype(tpl)>{} == 2);

            auto [xx, yy] = a.get_all_components();

            for (const auto& t : xx){
                REQUIRE(t == 1);
            }
            for (const auto& t : yy){
                REQUIRE(t == 2);
            }

            CHECK(xx[0] == 1);
            CHECK(yy[0] == 2);

        }

        SECTION("get_all_components non-const"){


            vectorField<2> a(scalarField(5, 1), scalarField(5, 2));
            auto [x,y] = a.get_all_components();

            for (auto& e : x){
                e = 4;
            }
            for (auto& e : y){
                e = 9;
            }

            auto [xx,yy] = a.get_all_components();
            CHECK(xx[0] == 4);
            CHECK(yy[0] == 9);

        }

    }


    SECTION("Arithmetic operations")
    {

        SECTION("plus"){
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));
            vectorField<2> c = a + b;

            auto [x,y] = c.get_all_components();
            for (const auto& t : x){
                REQUIRE(t == 4);
            }
            for (const auto& t : y){
                REQUIRE(t == 6);
            }

        }
        SECTION("multiplies"){
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));
            vectorField<2> c = a * b;

            auto [x,y] = c.get_all_components();
            for (const auto& t : x){
                REQUIRE(t == 3);
            }
            for (const auto& t : y){
                REQUIRE(t == 8);
            }

        }


        SECTION("dot")
        {
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));

            auto s = dot(a, b);
            CHECK(s[0] == 1 * 3 + 2 * 4);
            CHECK(s[1] == 1 * 3 + 2 * 4);

        }
        SECTION("mag")
        {
            vectorField<2> a(scalarField(10, 1), scalarField(10, 2));
            vectorField<2> b(scalarField(10, 3), scalarField(10, 4));

            auto s = dot(a, b);
            CHECK(s[0] == 1 * 3 + 2 * 4);
            CHECK(s[1] == 1 * 3 + 2 * 4);

        }




    }


}