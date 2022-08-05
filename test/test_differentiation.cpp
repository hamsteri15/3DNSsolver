#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "common/mdspan.hpp"

TEST_CASE("get_padding"){

    CHECK(get_padding<2>(CD2<0>{}) == std::array<size_t, 2>{1,0});
    CHECK(get_padding<2>(CD2<1>{}) == std::array<size_t, 2>{0,1});
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

        auto b = evaluate(a, dims, CD2<0>{});

        print(make_span(b, padded_dims));

        //print(b);

        CHECK(b[0] == 0);


        //std::vector<int> v();

    }






}