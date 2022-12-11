#pragma once

#include "jada.hpp"

static constexpr auto make_boundary_op_mirror(size_t padding){

    using namespace jada;

    auto op = [=](auto f){
        for (size_t i = 0; i < padding; ++i){
            f(index_type(i)) = f(-index_type(i));
        }
    };

    return op;

}

static constexpr void boundary_apply(auto span, auto dir, auto op){

    using namespace jada;

    auto new_op = [=](auto idx){
        auto handle = idxhandle_boundary_md_to_oned(span, idx, dir);
        op(handle);
    };

    for_each_boundary_index(dir, dimensions(span), new_op);


}
