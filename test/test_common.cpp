#include "catch.hpp"

#include "common/math.hpp"
#include "common/mdspan.hpp"

TEST_CASE("scalar"){

    CHECK(mag(-3.0) == 3.0);
    CHECK(mag(2.0) == 2.0);

}

TEST_CASE("scalarField"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(scalarField{});

    }

    SECTION("mag"){
        scalarField f(3, 3.0);
        f[1] = -2;
        scalarField r = mag(f);

        CHECK(r[0] == 3.0);
        CHECK(r[1] == 2.0);
        CHECK(r[2] == 3.0);
    }
}



TEST_CASE("Vector"){

    Vector<3> a = {1.0, 2.0, 3.0};
    Vector<3> b = {1.0, 2.0, 3.0};

    CHECK(a == b);
    CHECK(a + b == Vector<3>{2.0, 4.0, 6.0});
    CHECK(dot(a,b) == 1.0 + 2.0*2.0 + 3.0*3.0);
    CHECK(mag(Vector<3>{1,1,1}) == sqrt(3.0));
    CHECK(3.0 * a * 1.0 == Vector<3>{3.0, 6.0, 9.0});
}


TEST_CASE("VectorField"){

    SECTION("Constructors"){

    }

    SECTION("Mag"){
        vectorField<2> f(3);
        f[0] = Vector<2>{2,0};
        f[1] = Vector<2>{0,3};
        auto r = mag(f);
        CHECK(r[0] == 2.0);
        CHECK(r[1] == 3.0);
    }

    SECTION("Dot"){
        vectorField<2> a{Vector<2>{1,0}, Vector<2>{0, 2}};
        vectorField<2> b{Vector<2>{1,1}, Vector<2>{1, 2}};

        auto r = dot(a, b);
        CHECK(r[0] == 1.0);
        CHECK(r[1] == 4.0);

    }

}

/*
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

            auto s = mag(a);
            for (const auto& t : s){
                REQUIRE(t == std::sqrt(1*1+2*2));
            }


        }




    }


}
*/



TEST_CASE("mdspan tests"){


    SECTION("make_span"){

        scalarField f(10, 1.0);

        auto span = make_span(f, stdex::extents{2,5});

        CHECK(span(0, 3) == 1.0);
        CHECK(span(1, 4) == 1.0);
        span(1,4) = 43.0;
        CHECK(span(1,4) == 43.0);



    }




}