#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"


struct SomeOp{



};


template<class Ext, class Op>
auto make_padded_extent(Ext extent, Op op){

    (void) op;
    static constexpr size_t N = Ext::rank();

    std::array<size_t, N> arr{};

    for (size_t i = 0; i < N; ++i){

        arr[i] = extent.extent(i) + 2 * 1;

    }
    return Ext(arr);

}



template<class Ext, class Op>
auto make_indices(Ext extent, Op op){

    (void) op;
    static constexpr size_t N = Ext::rank();


    std::array<size_t, N> padding{};
    padding.fill(1); //This is wrong


    std::array<size_t, N> begin = padding;
    std::array<size_t, N> end{};

    for (size_t i = 0; i < N; ++i){
        end[i] = begin[i] + extent.extent(i);
    }


    return md_indices(begin, end);
}



template<class T, class Ext, class Op>
auto evaluate(const T& in, Ext extent, Op op){

    T out(in.size());

    auto padded = make_padded_extent(extent, op);

    const auto a = make_span(in, padded);
    auto b = make_span(out, padded);


    (void) a;
    (void) b;

    auto v = make_indices(extent, op);


    std::for_each(
        std::begin(v),
        std::end(v),
        [=](auto idx){
            b(idx) = a(shift<0, 1>(idx)) - a(shift<0, -1>(idx));
        }

    );


    return out;


}
