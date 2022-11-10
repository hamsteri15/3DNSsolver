#pragma once

#include "fields/volumetric_field.hpp"
#include "jada/utils.hpp"


auto compute_shift(auto padding, auto normal){
    
    static constexpr size_t N = decltype(padding)::rank();

    std::array<size_t, N> shift_amount;
    for (size_t i = 0; i < N; ++i){
        shift_amount[i] = padding.extent(i) * normal[i];
    }

    return shift_amount;
}

auto shift(auto idx, auto padding, auto normal){


    auto new_idx = tuple_to_array(idx);
    auto shift_amount = compute_shift(padding, normal);

    for (size_t i = 0; i < Rank<decltype(padding)>::value; ++i){
        new_idx[i] += shift_amount[i]; 
    }
    return new_idx;

}

template<class Span, size_t N>
void mirror(Span b_span, extents<N> padding, Vector<N> normal){
    runtime_assert(std::abs(normal.elementwise_sum()) == scalar(1),
                   "Only unit normal mirroring supported for now.");

    //TODO: this is inefficient
    for (auto idx : all_indices(b_span)){

        b_span(shift(idx, padding, normal)) 
        = b_span(tuple_to_array(idx));
    }

}


template <class ET, size_t N> void mirror(VolumetricField<ET, N>& f, Vector<N> normal) {

    auto b_span = boundary_subspan(f, normal);
    auto padding = f.padding();
    mirror(b_span, padding, normal);
}
template <class ET, size_t N> void mirror_all(VolumetricField<ET, N>& f) {

    for (size_t i = 0; i < N; ++i){
        Vector<N> normal{};
        normal[i] = 1.0;
        mirror(f, normal);
        normal[i] = -1.0;
        mirror(f, normal);
    }

}
