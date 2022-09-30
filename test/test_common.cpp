#include "catch.hpp"

#include "common/math.hpp"
#include "common/mdspan.hpp"
#include "common/extents.hpp"
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

TEST_CASE("extents"){

    SECTION("constructors"){

        REQUIRE_NOTHROW(extents<0>{});
        REQUIRE_NOTHROW(extents<2>{});
        REQUIRE_NOTHROW(extents<2>{4,4});
        REQUIRE_NOTHROW(extents<3>{4,4,0});

    }

}

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


    SECTION("shift"){

        auto t = md_indices(std::array{0,0,0}, std::array{3,3,2});

        auto [i1, j1, k1] = t[0];
        CHECK(i1 == 0);
        CHECK(j1 == 0);
        CHECK(k1 == 0);


        auto [i2, j2, k2] = shift<0,1>(t[0]);
        CHECK(i2 == 1);
        CHECK(j2 == 0);
        CHECK(k2 == 0);

        auto [i3, j3, k3] = shift<0,2>(t[0]);
        CHECK(i3 == 2);
        CHECK(j3 == 0);
        CHECK(k3 == 0);


        auto [i4, j4, k4] = shift<1, -1>(*(t.end() - 1));
        CHECK(i4 == 2);
        CHECK(j4 == 1);
        CHECK(k4 == 1);



    }



}


TEST_CASE("mdspan tests"){

    
    SECTION("Indexing a span"){
     



        /*
        auto s1 = make_span(a, extents<2>{2,5});

        CHECK(s1(std::array<size_t, 3>{1,1,1}) == 1);
        */
    }
    
    SECTION("extents"){

        REQUIRE_NOTHROW(extents<3>{3,3,3});

        std::array<size_t, 3> dims = {3,3,3};
        REQUIRE_NOTHROW(extents<3>{dims});

        extents<3> a{2,2,2};
        extents<3> b{2,3,2};
        extents<3> c{2,3,2};

        CHECK(a != b);
        CHECK(b == c);

        //auto c = a + b;

    }


    SECTION("make_span"){

        SECTION("vector tests"){

            std::vector<int> a(10, 1);
            const std::vector<int> b(10, 1);

            auto s1 = make_span(a, extents<2>{2,5});
            auto s2 = make_span(b, extents<2>{2,5});


            CHECK(s1(1,1) == s2(1,1));

            s1(1,1) = 43;
            CHECK(s1(1,1) == 43);

        }


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
            auto tt = get_array_from_tuple(t);
            span(tt) = 43;
        }

        for (auto t : all_indices(span)){
            auto tt = get_array_from_tuple(t);
            CHECK(span(tt) == 43);
        }


    }


    SECTION("differentiate"){

        std::vector<int> a(3*4, 1.0);
        const std::vector<int> b(3*4, 1.0);
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

TEST_CASE("subspan tests"){

    SECTION("make_subspan"){

        std::vector<int> a = 
        {
            0,0,0,0,
            0,1,1,0,
            0,1,1,0,
            0,0,0,0
        };
        auto s = make_span(a, extents<2>{4,4});
        auto ss = make_subspan(s, std::array<size_t,2>{1,1}, std::array<size_t,2>{3,3});
        CHECK(ss(0,0) == 1);

        CHECK(ss.extent(0) == 2);
        CHECK(ss.extent(1) == 2);

        ss(-1, -1) = 4;
        ss(2,2) = 7;
        ss(2,1) = 5;

        std::vector<int> correct = 
        {
            4,0,0,0,
            0,1,1,0,
            0,1,1,0,
            0,0,5,7
        };
        CHECK(a == correct);


    }

}