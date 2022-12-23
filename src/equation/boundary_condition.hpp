#pragma once

#include "fields/volumetric_field.hpp"
#include <jada.hpp>

static constexpr void boundary_apply(auto span, auto dir, auto op){

    using namespace jada;

    auto new_op = [=](auto idx){
        auto handle = idxhandle_boundary_md_to_oned(span, idx, dir);
        op(handle);
    };

    for_each_index(boundary_indices(dimensions(span), dir), new_op);
}




auto compute_shift(auto padding, auto normal){
    
    static constexpr size_t N = jada::rank(padding);

    std::array<size_t, N> shift_amount;
    for (size_t i = 0; i < N; ++i){
        shift_amount[i] = padding.extent(i) * normal[i];
    }

    return shift_amount;
}

auto shift(auto idx, auto padding, auto normal){


    auto new_idx = jada::tuple_to_array(idx);
    auto shift_amount = compute_shift(padding, normal);

    for (size_t i = 0; i < jada::rank(padding); ++i){
        new_idx[i] += shift_amount[i]; 
    }
    return new_idx;

}

template<class Span, size_t N>
void mirror(Span b_span, jada::extents<N> padding, Vector<N> normal){
    jada::runtime_assert(std::abs(normal.elementwise_sum()) == scalar(1),
                   "Only unit normal mirroring supported for now.");

    //TODO: this is inefficient
    for (auto idx : jada::all_indices(b_span)){

        b_span(shift(idx, padding, normal)) 
        = b_span(jada::tuple_to_array(idx));
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
