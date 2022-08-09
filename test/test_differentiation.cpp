#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"
#include "common/mdspan.hpp"


TEST_CASE("get_padding"){

    SECTION("Single op"){

        CHECK(get_padding<2>(CD2<0>{}) == std::array<size_t, 2>{1,0});
        CHECK(get_padding<2>(CD2<1>{}) == std::array<size_t, 2>{0,1});

    }
    SECTION("Multiple ops"){


        SECTION("Test 1"){
            auto tpl = std::make_tuple(CD2<0>{}, CD2<1>{}, CD2<2>{});
            auto arr = get_padding<3>(tpl);

            CHECK(arr == std::array<size_t, 3>{1, 1, 1});
        }
        SECTION("Test 2"){
            auto tpl = std::make_tuple(CD2<1>{}, CD2<1>{}, CD2<2>{});
            auto arr = get_padding<3>(tpl);

            CHECK(arr == std::array<size_t, 3>{0, 1, 1});
        }
        SECTION("Test 3"){
            auto tpl = std::make_tuple(CD2<0>{}, CD2<1>{}, CD2<2>{});
            auto arr = get_padding<5>(tpl);

            CHECK(arr == std::array<size_t, 5>{1, 1, 1, 0, 0});
        }

    }
}


TEST_CASE("make_padded_extent"){

    auto asd = make_padded_extent(extents<2>{2,2}, CD2<0>{});
    CHECK(asd.extent(0) == 4);
    CHECK(asd.extent(1) == 2);

}



TEST_CASE("cd-2"){




    SECTION("1d"){

        CD2<0> op;

        extents<2> dims{3, 4};
        extents<2> padded_dims = make_padded_extent(dims, op);


        std::vector<int> a(flat_size(padded_dims));

        auto b = evaluate_tiled(a, dims, CD2<0>{});

        print(make_span(b, padded_dims));

        //print(b);

        CHECK(b[0] == 0);


        //std::vector<int> v();

    }






}