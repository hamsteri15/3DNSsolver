#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"
#include "differentiation/eno.hpp"
#include "differentiation/weno.hpp"
#include "common/mdspan.hpp"

#include "test_helpers.hpp"


TEST_CASE("weno weights"){


    SECTION("weights_shu"){

        std::array<double, 3> w = weights_shu(1.0, 1.0, 1.0, 1.0, 1.0, std::array<double,3>{1.0, 1.0, 1.0});
        CHECK(w[0] == 1./3.);
        CHECK(w[1] == 1./3.);
        CHECK(w[2] == 1./3.);

    }

}
/*
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

*/



TEST_CASE("1D cd-2"){

    SECTION("evaluate_tiled 1"){

        d_CD2<0> op;
        extents<1> dims{7};

        std::vector<int> in(flat_size(dims));
        std::vector<int> out(flat_size(dims));
        auto s_in = make_span(in, dims);
        auto s_out = make_span(out, dims);
        
        set_linear<0>(s_in);
        
        evaluate_tiled(s_in, s_out, op);
        CHECK(out == std::vector<int>{0, 1, 1, 1, 1, 1, 0});
    }
    
    SECTION("evaluate_tiled 2"){

        d_CD2<0> op;
        size_t N = 3000;
        extents<1> dims{N};

        std::vector<int> in(flat_size(dims));
        std::vector<int> out(flat_size(dims));
        auto s_in = make_span(in, dims);
        auto s_out = make_span(out, dims);
        
        set_linear<0>(s_in);
        
        evaluate_tiled(s_in, s_out, op);

        for (size_t i = 1; i < N-2; ++i){
            REQUIRE(out.at(i) == 1);
        }
        CHECK(out.at(0) == 0);
        CHECK(out.at(N-1) == 0);

    }

    

}

TEST_CASE("2D cd-2"){

    
    SECTION("0-dir evaluate_tiled"){


        d_CD2<0> op;
        extents<2> dims{2 + 2*op.padding,3};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<0>(make_span(in, dims));

        evaluate_tiled(
            make_span(in, dims),
            make_span(out, dims),
            op
        );        

        CHECK(
            out == std::vector<int>
            {
                0, 0, 0,
                1, 1, 1,
                1, 1, 1,
                0, 0, 0
            }
        );
        
    }    
    
    SECTION("1-dir evaluate_tiled"){


        d_CD2<1> op;
        extents<2> dims{2,3 + 2*op.padding};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<1>(make_span(in, dims));

        evaluate_tiled(
            make_span(in, dims),
            make_span(out, dims),
            op
        );        


        CHECK(
            out == std::vector<int>
            {
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0
            }
        );
        
    }    
    
    SECTION("Both dirs evaluate_tiled"){


        d_CD2<0> op0;
        d_CD2<1> op1;
        extents<2> dims{2 + 2*op0.padding,3 + 2*op1.padding};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<0>(make_span(in, dims));

        evaluate_tiled(
            make_span(in, dims),
            make_span(out, dims),
            op0
        );        


        CHECK(
            out == std::vector<int>
            {
                0, 0, 0, 0, 0,
                1, 1, 1, 1, 1,
                1, 1, 1, 1, 1,
                0, 0, 0, 0, 0
            }
        );

        std::transform(out.begin(), out.end(), out.begin(), [](auto a){(void) a; return 0.0;});

        set_linear<1>(make_span(in, dims));

        evaluate_tiled(
            make_span(in, dims),
            make_span(out, dims),
            op1
        );        
        
        CHECK(
            out == std::vector<int>
            {
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 1, 1, 0
            }
        );

    }
        


}