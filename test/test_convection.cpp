#include "catch.hpp"

#include "convection/convection_flux.hpp"


TEST_CASE("ConvectionFlux1D"){

    SECTION("Constructors"){

        REQUIRE_NOTHROW(ConvectionFlux1D{});

    }

}
