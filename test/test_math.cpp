#include "catch.hpp"

#include "math/scalar.hpp"
#include "math/vector.hpp"
#include "math/math_vector.hpp"

TEST_CASE("Test scalar"){
    CHECK(mag(-3.0) == 3.0);
    CHECK(mag(2.0) == 2.0);
}

TEST_CASE("Test math vector"){


    SECTION("Constructors"){

        MathVector<int,2> v1 = {1,2};
        auto v2 = v1;
        MathVector<int,2> v3(v1);
        MathVector<int,2> v4;

        REQUIRE_THROWS(MathVector<int,2>{1,2,3});

        CHECK(v1[0] == 1);
        CHECK(v2[0] == 1);
        CHECK(v3[0] == 1);
        CHECK(v4[0] == 0);
        CHECK(v4[1] == 0);


    }

    SECTION("Operations"){

        MathVector<int, 2> a = {1,2};
        MathVector<int, 2> b = {3,4};


        auto c = a * 3 + b - a / 3;

        CHECK(c[0] == 1 * 3 + 3 - 1 / 3);


        CHECK(a[0] == 1);


        CHECK(a != b);
        a = c;
        CHECK(a == c);
        CHECK(a[0] == c[0]);
        CHECK(a[1] == c[1]);

        for (auto& elem : a){
            elem = 43;
        }
        CHECK(a[0] == 43);
        CHECK(a[1] == 43);

        CHECK(std::accumulate(std::begin(a), std::end(a), 0) == 2*43);
        CHECK(std::accumulate(std::cbegin(a), std::cend(a), 0) == 2*43);
        CHECK(std::accumulate(std::rbegin(a), std::rend(a), 0) == 2*43);
        CHECK(std::accumulate(std::crbegin(a), std::crend(a), 0) == 2*43);


        a[0] = 1;
        CHECK(a.min() == 1);
        CHECK(a.max() == 43);
        a[0] = -2;
        a[1] = -2;
        CHECK(a.abs() == MathVector<int, 2>{2,2});

    }

    
    SECTION("dot"){
        Vector<3> a = {1.0, 2.0, 3.0};
        Vector<3> b = {1.0, 2.0, 3.0};
        CHECK(dot(a,b) == 1.0 + 2.0*2.0 + 3.0*3.0);
    }
    
    SECTION("mag"){
        CHECK(mag(Vector<3>{1,1,1}) == sqrt(3.0));
    }
    
    SECTION("operator=="){
        Vector<3> a = {1.0, 2.0, 3.0};
        Vector<3> b = {1.0, 2.0, 3.0};
        CHECK(a == b);
        CHECK(a + b == Vector<3>{2.0, 4.0, 6.0});
        CHECK(3.0 * a * 1.0 == Vector<3>{3.0, 6.0, 9.0});
    }
    
}


