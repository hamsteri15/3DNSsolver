#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"
#include "common/mdspan.hpp"
#include "common/make_field.hpp"

TEST_CASE("get_padding"){

    SECTION("Single op"){

        CHECK(get_padding<2>(dd_CD2<0>{}) == std::array<size_t, 2>{1,0});
        CHECK(get_padding<2>(dd_CD2<1>{}) == std::array<size_t, 2>{0,1});

    }
    SECTION("Multiple ops"){


        SECTION("Test 1"){
            auto tpl = std::make_tuple(dd_CD2<0>{}, dd_CD2<1>{}, dd_CD2<2>{});
            auto arr = get_padding<3>(tpl);

            CHECK(arr == std::array<size_t, 3>{1, 1, 1});
        }
        SECTION("Test 2"){
            auto tpl = std::make_tuple(dd_CD2<1>{}, dd_CD2<1>{}, dd_CD2<2>{});
            auto arr = get_padding<3>(tpl);

            CHECK(arr == std::array<size_t, 3>{0, 1, 1});
        }
        SECTION("Test 3"){
            auto tpl = std::make_tuple(dd_CD2<0>{}, dd_CD2<1>{}, dd_CD2<2>{});
            auto arr = get_padding<5>(tpl);

            CHECK(arr == std::array<size_t, 5>{1, 1, 1, 0, 0});
        }

    }
}


TEST_CASE("make_padded_extent"){

    auto asd = make_padded_extent(extents<2>{2,2}, dd_CD2<0>{});
    CHECK(asd.extent(0) == 4);
    CHECK(asd.extent(1) == 2);

}

template<size_t Dir, size_t N, class T>
void set_linear(span<T, N> s){

    auto indices = all_indices(s);
    for (auto idx : indices){
        auto ii = std::get<Dir>(idx);
        s(idx) = T(ii);
    }

}




TEST_CASE("cd-2"){

    SECTION("evaluate_tiled 1"){

        d_CD2<0> op;
        extents<1> interior{5};
        extents<1> padded = make_padded_extent(interior, op);
        CHECK(padded == extents<1>{7});

        std::vector<int> in(flat_size(padded));
        std::vector<int> out(flat_size(padded));
        auto s_in = make_span(in, padded);
        auto s_out = make_span(out, padded);
        
        set_linear<0>(s_in);
        
        evaluate_tiled(s_in, s_out, op);
        CHECK(out == std::vector<int>{0, 2, 2, 2, 2, 2, 0});


    }
    SECTION("evaluate tiled 2"){


        d_CD2<0> op;
        extents<1> interior{5};

        scalarField in = make_scalar_field(interior, op);

        set_linear<0>(make_span(in, make_padded_extent(interior, op)));

        scalarField out = evaluate_tiled(in, interior, op);

        CHECK(out == scalarField{0, 2, 2, 2, 2, 2, 0});

    }
    


}