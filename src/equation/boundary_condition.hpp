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




