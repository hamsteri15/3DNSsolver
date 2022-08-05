#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "common/mdspan.hpp"


TEST_CASE("make_padded_extent"){


    auto asd = make_padded_extent(extents<2>{2,2}, SomeOp{});

    CHECK(asd.extent(0) == 4);
    CHECK(asd.extent(1) == 4);

}


TEST_CASE("cd-2"){



    SECTION("1d"){


        extents<2> dims{3, 4};
        extents<2> padding{1,1};

        size_t size = (3 + 2* 1) * (4 + 2* 1);

        std::vector<int> a(size);



        auto b = evaluate(a, dims, padding);

        //print(b);

        CHECK(b[0] == 0);


        //std::vector<int> v();




    }






}