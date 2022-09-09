#pragma once

#include "common/runtime_assert.hpp"
#include "equation/volumetric_field.hpp"
#include "common/utils.hpp"


auto compute_shift(auto padding, auto normal){
    
    static constexpr size_t N = decltype(padding)::rank();

    std::array<size_t, N> shift_amount;
    for (size_t i = 0; i < N; ++i){
        shift_amount[i] = padding.extent(i) * normal[i];
    }

    return shift_amount;
}

auto shift(auto idx, auto padding, auto normal){


    auto new_idx = get_array_from_tuple(idx);
    auto shift_amount = compute_shift(padding, normal);

    for (size_t i = 0; i < rank(padding); ++i){
        new_idx[i] += shift_amount[i]; 
    }
    return new_idx;

}


template <class ET, size_t N> void mirror(VolumetricField<ET, N>& f, Vector<N> normal) {

    runtime_assert(std::abs(normal.elementwise_sum()) == scalar(1),
                   "Only unit normal mirroring supported for now.");



    auto b_span = boundary_subspan(f, normal);

    auto padding = f.padding();

    //TODO: this is inefficient
    for (auto idx : all_indices(b_span)){

        b_span(shift(idx, padding, normal)) 
        = b_span(get_array_from_tuple(idx));
    }


}
