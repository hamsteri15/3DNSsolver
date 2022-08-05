#pragma once

#include "common/mdspan.hpp"
#include "common/loop.hpp"


template<size_t N, class Op>
std::array<size_t, N> get_padding(Op op){

    (void) op;

    std::array<size_t, N> ret{};
    ret[Op::direction] = 1;

    return ret;

}


template<size_t Dir>
struct CD2{


    static constexpr size_t direction = Dir;

    auto operator()(auto span, auto idx) const{

        (void) span;
        (void) idx;
        return 43;
    }

};


template<class Ext, class Op>
auto make_padded_extent(Ext extent, Op op){


    static constexpr size_t N = Ext::rank();

    std::array<size_t, N> arr{};
    auto padding = get_padding<N>(op);


    for (size_t i = 0; i < N; ++i){
        arr[i] = extent.extent(i) + 2 * padding[i];
    }

    return Ext(arr);

}



template<class Ext, class Op>
auto make_indices(Ext extent, Op op){


    static constexpr size_t N = Ext::rank();

    auto padding = get_padding<N>(op);

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

    auto v = make_indices(extent, op);


    std::for_each(
        std::begin(v),
        std::end(v),
        [=](auto idx){
            //b(idx) = a(shift<0, 1>(idx)) - a(shift<0, -1>(idx));
            b(idx) = op(a, idx);

            //b(idx) = 43;
        }

    );


    return out;


}
