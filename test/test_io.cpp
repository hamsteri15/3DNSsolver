#include "catch.hpp"

#include "test_helpers.hpp"
#include "io/write.hpp"
#include "io/make_file.hpp"

#include "test_helpers.hpp"

TEST_CASE("Test write"){

    using namespace H5Wrapper;

    std::string fname = "test_file1.h5";
    auto hf = make_file(fname);


    
    auto eq = make_euler_equation<2>(extents<2>{5, 4}, extents<2>{1,1});
    assign_shocktube<0>(eq);

    write(hf, eq.primitive_variables().rho);


}