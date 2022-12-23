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


