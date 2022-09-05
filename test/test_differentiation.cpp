#include "catch.hpp"

#include "differentiation/cd-n.hpp"
#include "differentiation/evaluate_tiled.hpp"
#include "differentiation/eno.hpp"
#include "differentiation/weno.hpp"
#include "common/mdspan.hpp"
#include "common/make_field.hpp"

#include "test_helpers.hpp"


TEST_CASE("weno weights"){


    SECTION("weights_shu"){

        std::array<double, 3> w = weights_shu(1.0, 1.0, 1.0, 1.0, 1.0, std::array<double,3>{1.0, 1.0, 1.0});
        CHECK(w[0] == 1./3.);
        CHECK(w[1] == 1./3.);
        CHECK(w[2] == 1./3.);

    }

}

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





TEST_CASE("1D cd-2"){

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

    

}

TEST_CASE("2D cd-2"){

    SECTION("0-dir evaluate_tiled"){


        d_CD2<0> op;
        extents<2> interior{2,3};

        scalarField in = make_scalar_field(interior, op);
        scalarField out = make_scalar_field(interior, op);

        set_linear<0>(make_span(in, make_padded_extent(interior, op)));

        evaluate_tiled(
            make_span(in, make_padded_extent(interior, op)),
            make_span(out, make_padded_extent(interior, op)),
            op
        );        

        CHECK(
            out == scalarField
            {
                0, 0, 0,
                2, 2, 2,
                2, 2, 2,
                0, 0, 0
            }
        );
        
    }    
    
    SECTION("1-dir evaluate_tiled"){


        d_CD2<1> op;
        extents<2> interior{2,3};

        scalarField in = make_scalar_field(interior, op);
        scalarField out = make_scalar_field(interior, op);

        set_linear<1>(make_span(in, make_padded_extent(interior, op)));

        evaluate_tiled(
            make_span(in, make_padded_extent(interior, op)),
            make_span(out, make_padded_extent(interior, op)),
            op
        );        


        CHECK(
            out == scalarField
            {
                0, 2, 2, 2, 0,
                0, 2, 2, 2, 0
            }
        );
        
    }    
    
    SECTION("Both dirs evaluate_tiled"){


        d_CD2<0> op0;
        d_CD2<1> op1;
        extents<2> interior{2,3};

        scalarField in = make_scalar_field(interior, op0, op1);
        scalarField out = make_scalar_field(interior, op0, op1);

        set_linear<0>(make_span(in, make_padded_extent(interior, op0, op1)));

        evaluate_tiled(
            make_span(in, make_padded_extent(interior, op0, op1)),
            make_span(out, make_padded_extent(interior, op0, op1)),
            op0
        );        


        CHECK(
            out == scalarField
            {
                0, 0, 0, 0, 0,
                2, 2, 2, 2, 2,
                2, 2, 2, 2, 2,
                0, 0, 0, 0, 0
            }
        );

        std::transform(out.begin(), out.end(), out.begin(), [](auto a){(void) a; return 0.0;});

        set_linear<1>(make_span(in, make_padded_extent(interior, op0, op1)));

        evaluate_tiled(
            make_span(in, make_padded_extent(interior, op0, op1)),
            make_span(out, make_padded_extent(interior, op0, op1)),
            op1
        );        
        
        CHECK(
            out == scalarField
            {
                0, 2, 2, 2, 0,
                0, 2, 2, 2, 0,
                0, 2, 2, 2, 0,
                0, 2, 2, 2, 0
            }
        );

    }    


}