#include "catch.hpp"

#include "common/math.hpp"
#include "common/mdspan.hpp"
#include "common/loop.hpp"


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

TEST_CASE("md_indices tests"){

    SECTION("First"){

        auto t = md_indices(std::array{0,0,0}, std::array{3,3,2});

        auto [i1,j1,k1] = *t.begin();
        CHECK(i1 == 0);
        CHECK(j1 == 0);
        CHECK(k1 == 0);

        auto [i2, j2, k2] = *(t.begin() + 1);
        CHECK(i2 == 0);
        CHECK(j2 == 0);
        CHECK(k2 == 1);

        auto [i3, j3, k3] = *(t.end() - 1);
        CHECK(i3 == 2);
        CHECK(j3 == 2);
        CHECK(k3 == 1);

    }


}

TEST_CASE("mdspan tests"){

    SECTION("extents"){

        REQUIRE_NOTHROW(extents<3>{3,3,3});

        std::array<size_t, 3> dims = {3,3,3};
        REQUIRE_NOTHROW(extents<3>{dims});

        extents<3> a{2,2,2};
        extents<3> b{2,3,2};

        //auto c = a + b;

    }


    SECTION("make_span"){

        SECTION("equal size span"){
            scalarField f(10, 1.0);

            auto span = make_span(f, extents<2>{2,5});

            CHECK(span(0, 3) == 1.0);
            CHECK(span(1, 4) == 1.0);
            span(1,4) = 43.0;
            CHECK(span(1,4) == 43.0);

            static_assert(rank(span) == size_t(2), "rank() not found");
            CHECK(rank(span) == size_t(2));

            CHECK(span(std::array<size_t,2>{1,1}) == 1.0);

        }
        
        SECTION("non-equal size span"){
            scalarField f(10, 1.0);

            REQUIRE_THROWS(make_span(f, extents<2>{3,5}));
            
        }



    }


    SECTION("all_indices"){

        std::vector<int> f(10);

        auto span = make_span(f, extents<2>{2,5});
       

        std::vector<index_type> is;
        std::vector<index_type> js;

        for (auto [i, j] : all_indices(span)){
            is.push_back(i);
            js.push_back(j);
        }

        CHECK(is == std::vector<index_type>{0,0,0,0,0,1,1,1,1,1});
        CHECK(js == std::vector<index_type>{0,1,2,3,4,0,1,2,3,4});
 
    }



    SECTION("index span"){
        std::vector<int> f(10, 1.0);
        auto span = make_span(f, extents<2>{2,5});

        for (auto t : all_indices(span)){
            span(t) = 43;
        }
        
        for (auto t : all_indices(span)){
            CHECK(span(t) == 43);
        }


    }
    

    SECTION("differentiate"){

        std::vector<int> a(3*4, 1.0);
        std::vector<int> b(3*4, 1.0);
        auto aa = make_span(a, extents<2>{3,4});
        auto bb = make_span(b, extents<2>{3,4});


        auto v = md_indices(std::array{1,1}, std::array{2,3});

        std::for_each(
            std::begin(v),
            std::end(v),
            [=] (auto idx){
                auto [i, j] = idx;
                aa(i, j) = bb(i-1, j) + bb(i+1, j);
            } 
        );




    }




}