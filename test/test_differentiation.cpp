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

