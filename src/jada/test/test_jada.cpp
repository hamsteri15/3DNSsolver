#include "catch.hpp"

#include "include/jada.hpp"
#include "test.hpp"

TEST_CASE("rank"){


    CHECK(jada::rank(std::make_tuple(1,2,3)) == 3);
    CHECK(jada::rank(std::array<size_t, 2>{}) == 2);
    CHECK(jada::rank(std::array<int, 2>{}) == 2);
    //CHECK(jada::rank(std::make_tuple(1,2, 3.43)) == 3);

}

TEST_CASE("extents"){

    SECTION("constructors"){

        REQUIRE_NOTHROW(extents<0>{});
        REQUIRE_NOTHROW(extents<2>{});
        REQUIRE_NOTHROW(extents<2>{4,4});
        REQUIRE_NOTHROW(extents<3>{4,4,0});

    }

    SECTION("flat_size"){

        CHECK(flat_size(extents<3>{1,2,3}) == 1*2*3);
        CHECK(flat_size(extents<3>{1,0,3}) == 0);

    }

    SECTION("indices_in_bounds"){

        auto tpl = std::make_tuple(size_t(1), size_t(2), size_t(3));
        auto ext1 = make_extent(std::array<size_t, 3>{4,4,4});
        auto ext2 = make_extent(std::array<size_t, 3>{0,0,0});

        CHECK(indices_in_bounds(tpl, ext1) == true);
        CHECK(indices_in_bounds(tpl, ext2) == false);

        std::array<size_t, 3> arr{1,2,3};

        CHECK(indices_in_bounds(arr, ext1) == true);

    }


}

TEST_CASE("md_indices tests"){

    SECTION("First"){

        auto t = md_indices(std::array{0,0,0}, std::array{3,3,2});

        auto [i1,j1,k1] = *t.begin();
        CHECK(i1 == 0);
        CHECK(j1 == 0);
        CHECK(k1 == 0);

        
        auto [i2, j2, k2] = *(t.begin() + 1);
        CHECK(i2 == 0);
        CHECK(j2 == 0);
        CHECK(k2 == 1);

        auto [i3, j3, k3] = *(t.end() - 1);
        CHECK(i3 == 2);
        CHECK(j3 == 2);
        CHECK(k3 == 1);
        
    }


}


TEST_CASE("mdspan tests"){

    
    SECTION("Indexing a span"){
     
        /*
        auto s1 = jada::make_span(a, extents<2>{2,5});

        CHECK(s1(std::array<size_t, 3>{1,1,1}) == 1);
        */
    }
    
    SECTION("extents"){

        REQUIRE_NOTHROW(extents<3>{3,3,3});

        std::array<size_t, 3> dims = {3,3,3};
        REQUIRE_NOTHROW(extents<3>{dims});

        extents<3> a{2,2,2};
        extents<3> b{2,3,2};
        extents<3> c{2,3,2};

        CHECK(a != b);
        CHECK(b == c);

        //auto c = a + b;

    }


    SECTION("jada::make_span"){

        SECTION("vector tests"){

            std::vector<int> a(10, 1);
            const std::vector<int> b(10, 1);

            auto s1 = jada::make_span(a, extents<2>{2,5});
            auto s2 = jada::make_span(b, extents<2>{2,5});
            //auto s3 = jada::make_span(a, std::array<size_t, 2>{2,5});

            CHECK(s1(1,1) == s2(1,1));

            s1(1,1) = 43;
            CHECK(s1(1,1) == 43);
            //CHECK(s3(1,1) == 1);
        }

        

        SECTION("equal size span"){
            std::vector<double> f(10, 1.0);

            auto span = jada::make_span(f, extents<2>{2,5});

            CHECK(span(0, 3) == 1.0);
            CHECK(span(1, 4) == 1.0);
            span(1,4) = 43.0;
            CHECK(span(1,4) == 43.0);

            static_assert(jada::rank(span)== size_t(2), "jada::rank() not found");
            CHECK(jada::rank(span) == size_t(2));

            CHECK(span(std::array<size_t,2>{1,1}) == 1.0);

        }

        SECTION("non-equal size span"){
            std::vector<double> f(10, 1.0);

            REQUIRE_THROWS(jada::make_span(f, extents<2>{3,5}));

        }



    }


    SECTION("jada::all_indices"){

        std::vector<int> f(10);

        auto span = jada::make_span(f, extents<2>{2,5});


        std::vector<int> is;
        std::vector<int> js;

        for (auto [i, j] : jada::all_indices(span)){
            is.push_back(i);
            js.push_back(j);
        }

        CHECK(is == std::vector<int>{0,0,0,0,0,1,1,1,1,1});
        CHECK(js == std::vector<int>{0,1,2,3,4,0,1,2,3,4});

    }



    SECTION("index span"){
        std::vector<int> f(10, 1.0);
        auto span = jada::make_span(f, extents<2>{2,5});

        for (auto t : jada::all_indices(span)){
            auto tt = jada::tuple_to_array(t);
            span(tt) = 43;
        }

        for (auto t : jada::all_indices(span)){
            auto tt = jada::tuple_to_array(t);
            CHECK(span(tt) == 43);
        }


    }


    SECTION("differentiate"){

        std::vector<int> a(3*4, 1.0);
        const std::vector<int> b(3*4, 1.0);
        auto aa = jada::make_span(a, extents<2>{3,4});
        auto bb = jada::make_span(b, extents<2>{3,4});


        auto v = md_indices(std::array{1,1}, std::array{2,3});

        std::for_each(
            std::begin(v),
            std::end(v),
            [=] (auto idx){
                auto [i, j] = idx;

                aa(i, j) = bb(i-1, j) + bb(i+1, j);
            }
        );




    }

}

TEST_CASE("subspan tests"){

    SECTION("jada::make_subspan"){

        std::vector<int> a = 
        {
            0,0,0,0,
            0,1,1,0,
            0,1,1,0,
            0,0,0,0
        };
        auto s = jada::make_span(a, extents<2>{4,4});
        auto ss = jada::make_subspan(s, std::array<size_t,2>{1,1}, std::array<size_t,2>{3,3});
        CHECK(ss(0,0) == 1);

        CHECK(ss.extent(0) == 2);
        CHECK(ss.extent(1) == 2);

        ss(-1, -1) = 4;
        ss(2,2) = 7;
        ss(2,1) = 5;

        std::vector<int> correct = 
        {
            4,0,0,0,
            0,1,1,0,
            0,1,1,0,
            0,0,5,7
        };
        CHECK(a == correct);


    }
    
    SECTION("rank reduction subspan"){

        std::vector<int> a = 
        {
            1,  2,  3,  4,
            5,  6,  7,  8,
            9,  10, 11, 12,
            13, 14, 15, 16
        };
        auto s = jada::make_span(a, extents<2>{4,4});

        auto ss = stdex::submdspan(
            s,
            0, //j = 0
            std::pair(0, 3) // i = 0 ... 3
        );

        CHECK(ss(0) == 1);
        CHECK(ss(1) == 2);
        CHECK(ss(2) == 3);

    }



    SECTION("make_tiled_subspan"){
        
        SECTION("with given begin and end"){
            
            std::vector<int> a = 
            {
                1,  2,  3,  4,
                5,  6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16
            };
            auto s = jada::make_span(a, extents<2>{4,4});

            std::array<size_t, 2> begin1 = {2,1};
            std::array<size_t, 2> end1 = {2,3};


            auto ss = detail::make_tiled_subspan<1>(s, begin1, end1);

            CHECK(ss(0) == 10);
            CHECK(ss(1) == 11);
            CHECK(ss(2) == 12);
            
            std::array<size_t, 2> begin2 = {1,2};
            std::array<size_t, 2> end2 = {3,2};

            
            auto ss2 = detail::make_tiled_subspan<0>(s, begin2, end2);

            CHECK(ss2(0) == 7);
            CHECK(ss2(1) == 11);
            CHECK(ss2(2) == 15);
            
        }
        
        SECTION("with given begin and width"){
            
            std::vector<int> a = 
            {
                1,  2,  3,  4,
                5,  6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16
            };
            auto s = jada::make_span(a, extents<2>{4,4});


            auto ss = make_tiled_subspan<1>(s, std::make_tuple(2,1), size_t(2));

            CHECK(ss(0) == 10);
            CHECK(ss(1) == 11);
            CHECK(ss(2) == 12);
            
            
            auto ss2 = make_tiled_subspan<0>(s, std::array<size_t, 2>{1,2}, size_t(2));

            CHECK(ss2(0) == 7);
            CHECK(ss2(1) == 11);
            CHECK(ss2(2) == 15);
            
        }
        
        SECTION("with only center index given"){
            
            std::vector<int> a = 
            {
                1,  2,  3,  4,
                5,  6,  7,  8,
                9,  10, 11, 12,
                13, 14, 15, 16
            };
            auto s = jada::make_span(a, extents<2>{4,4});

            std::array<size_t, 2> center = {2,2};


            auto ss = make_tiled_subspan<1>(s, center);

            CHECK(ss(-1) == 10);
            CHECK(ss(0) == 11);
            CHECK(ss(1) == 12);
            
            
            auto ss2 = make_tiled_subspan<0>(s, center);

            CHECK(ss2(-1) == 7);
            CHECK(ss2(0) == 11);
            CHECK(ss2(1) == 15);
            
        }
    }
}


TEST_CASE("1D cd-2"){

    SECTION("evaluate_tiled 1"){

        d_CD2 op;
        extents<1> dims{7};

        std::vector<int> in(flat_size(dims));
        std::vector<int> out(flat_size(dims));
        auto s_in = jada::make_span(in, dims);
        auto s_out = jada::make_span(out, dims);
        
        set_linear<0>(s_in);
        
        evaluate_tiled<0>(s_in, s_out, op);
        CHECK(out == std::vector<int>{0, 1, 1, 1, 1, 1, 0});
    }
    
    SECTION("evaluate_tiled 2"){

        d_CD2 op;
        size_t N = 3000;
        extents<1> dims{N};

        std::vector<int> in(flat_size(dims));
        std::vector<int> out(flat_size(dims));
        auto s_in = jada::make_span(in, dims);
        auto s_out = jada::make_span(out, dims);
        
        set_linear<0>(s_in);
        
        evaluate_tiled<0>(s_in, s_out, op);

        for (size_t i = 1; i < N-2; ++i){
            REQUIRE(out.at(i) == 1);
        }
        CHECK(out.at(0) == 0);
        CHECK(out.at(N-1) == 0);

    }

    

}

TEST_CASE("2D cd-2"){

    
    SECTION("0-dir evaluate_tiled"){


        d_CD2 op;
        extents<2> dims{2 + 2*op.padding,3};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<0>(jada::make_span(in, dims));

        evaluate_tiled<0>(
            jada::make_span(in, dims),
            jada::make_span(out, dims),
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


        d_CD2 op;
        extents<2> dims{2,3 + 2*op.padding};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<1>(jada::make_span(in, dims));

        evaluate_tiled<1>(
            jada::make_span(in, dims),
            jada::make_span(out, dims),
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


        d_CD2 op0;
        d_CD2 op1;
        extents<2> dims{2 + 2*op0.padding,3 + 2*op1.padding};

        std::vector<int> in(flat_size(dims), 0);
        std::vector<int> out(flat_size(dims), 0);

        set_linear<0>(jada::make_span(in, dims));

        evaluate_tiled<0>(
            jada::make_span(in, dims),
            jada::make_span(out, dims),
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

        set_linear<1>(jada::make_span(in, dims));

        evaluate_tiled<1>(
            jada::make_span(in, dims),
            jada::make_span(out, dims),
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


TEST_CASE("Block neighbours"){



    //ensures that same comparator is used for all array of array comparisons
    auto comp = [](auto lhs, auto rhs) {
        return std::lexicographical_compare
        (
            lhs.begin(),
            lhs.end(),
            rhs.begin(),
            rhs.end()
        );
    };


    SECTION("Neighbours"){

        REQUIRE_NOTHROW(Neighbours<3, ConnectivityType::Star>());
        REQUIRE_NOTHROW(Neighbours<3, ConnectivityType::Box>());

        CHECK(is_unique(Neighbours<3, ConnectivityType::Box>().get()));
        CHECK(is_unique(Neighbours<2, ConnectivityType::Star>().get()));
        CHECK(is_unique(Neighbours<4, ConnectivityType::Box>().get()));



        Neighbours<2, ConnectivityType::Star> ns1;
        Neighbours<2, ConnectivityType::Box> ns2;

//        static constexpr auto ns_const = Neighbours<2, ConnectivityType::Star>();

        constexpr Neighbours<2, ConnectivityType::Star> ns_const;


        static_assert(Neighbours<2, ConnectivityType::Star>::idx({0,1}) == 0, "Neighbours not constexpr");

        CHECK(ns1.idx({0,1}) == 0);
        CHECK(ns2.idx({0,1}) == 0);
        static_assert(ns_const.idx({0,1}) == 0, "Neighbours not constexpr");

    }


    

    SECTION("Star connectivity"){

        SECTION("1D") {
            auto test = Neighbours<1, ConnectivityType::Star>().get();

            std::array<direction<1>, 2> correct
            {
                direction<1>{1}, direction<1>{-1}
            };

            std::sort(test.begin(), test.end());
            std::sort(correct.begin(), correct.end());

            CHECK(test == correct);

        }


        SECTION("2D") {

            using namespace Catch::Matchers;

            std::array<direction<2>, 4> test = Neighbours<2, ConnectivityType::Star>().get();

            std::array<direction<2>, 4> correct
            {
                direction<2>{1, 0},
                direction<2>{0, 1},
                direction<2>{-1, 0},
                direction<2>{0, -1}
            };

            std::sort(test.begin(), test.end(), comp);
            std::sort(correct.begin(), correct.end(), comp);

            CHECK(test == correct);

            test[2][0] = 3;
            CHECK(test != correct);

        }


        SECTION("3D") {
            auto test = Neighbours<3, ConnectivityType::Star>().get();

            std::array<direction<3>, 6> correct
            {
                direction<3>{1, 0, 0},
                direction<3>{0, 1, 0},
                direction<3>{0, 0, 1},
                direction<3>{-1, 0, 0},
                direction<3>{0, -1, 0},
                direction<3>{0, 0, -1}

            };

            std::sort(test.begin(), test.end(), comp);
            std::sort(correct.begin(), correct.end(), comp);

            CHECK(test == correct);

            test[2][0] = 3;
            CHECK(test != correct);


        }

    }



    SECTION("Box connecitvity"){

        SECTION("1D") {
            auto test = Neighbours<1, ConnectivityType::Box>().get();

            std::array<direction<1>, 2> correct
            {
                direction<1>{1}, direction<1>{-1}
            };

            std::sort(test.begin(), test.end(), comp);
            std::sort(correct.begin(), correct.end(), comp);

            CHECK(test == correct);


        }


        SECTION("2D") {
            auto test = Neighbours<2, ConnectivityType::Box>().get();

            std::array<direction<2>, 8> correct
            {
                direction<2>{1, 0},
                direction<2>{0, 1},
                direction<2>{1, 1},
                direction<2>{-1, 0},
                direction<2>{0, -1},
                direction<2>{-1, -1},
                direction<2>{-1, 1},
                direction<2>{1, -1}

            };

            std::sort(test.begin(), test.end(), comp);
            std::sort(correct.begin(), correct.end(), comp);

            CHECK(test == correct);



        }

    }
    

}

TEST_CASE("Test decomposition"){

    SECTION("get_offset"){

        SECTION("Even splitting"){
            std::array<size_t, 1> global_grid_dims  = {4};
            std::array<size_t, 1> coord_dims = {4};

            CHECK(offset(std::array<size_t, 1>{0}, coord_dims, global_grid_dims) == std::array<size_t, 1>{0});
            CHECK(offset(std::array<size_t, 1>{1}, coord_dims, global_grid_dims) == std::array<size_t, 1>{1});
            CHECK(offset(std::array<size_t, 1>{2}, coord_dims, global_grid_dims) == std::array<size_t, 1>{2});
            CHECK(offset(std::array<size_t, 1>{3}, coord_dims, global_grid_dims) == std::array<size_t, 1>{3});

        }
        SECTION("Uneven splitting"){
            std::array<size_t, 1> global_grid_dims  = {5};
            std::array<size_t, 1> coord_dims = {4};

            CHECK(offset(std::array<size_t, 1>{0}, coord_dims, global_grid_dims) == std::array<size_t, 1>{0});
            CHECK(offset(std::array<size_t, 1>{1}, coord_dims, global_grid_dims) == std::array<size_t, 1>{1});
            CHECK(offset(std::array<size_t, 1>{2}, coord_dims, global_grid_dims) == std::array<size_t, 1>{2});
            CHECK(offset(std::array<size_t, 1>{3}, coord_dims, global_grid_dims) == std::array<size_t, 1>{3});

            REQUIRE_THROWS(offset(std::array<size_t, 1>{4}, coord_dims, global_grid_dims));

        }
        
    }


    
    SECTION("local_extent()") {


        SECTION("Even splitting"){
            std::array<size_t, 3> global_grid_dims  = {4,4,4};
            std::array<size_t, 3> coord_dims = {4,4,4};

            CHECK(local_dimensions(std::array<size_t, 3>{0,0,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{1,1,1}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{0,1,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{3,1,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});

            REQUIRE_THROWS(local_dimensions(std::array<size_t, 3>{4,0,0}, coord_dims, global_grid_dims));

        }
        SECTION("Uneven splitting"){
            std::array<size_t, 3> global_grid_dims  = {4,5,4};
            std::array<size_t, 3> coord_dims = {4,4,4};

            CHECK(local_dimensions(std::array<size_t, 3>{0,0,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{1,1,1}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{0,1,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{3,1,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,1,1});
            CHECK(local_dimensions(std::array<size_t, 3>{3,3,0}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,2,1});
            CHECK(local_dimensions(std::array<size_t, 3>{2,3,1}, coord_dims, global_grid_dims) == std::array<size_t, 3>{1,2,1});


        }
        
    }
}


