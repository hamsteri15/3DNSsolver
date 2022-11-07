#pragma once

#include "equation/euler.hpp"

void for_each_index(auto span, auto op){
    for (auto idx : all_indices(span)){
        span(tuple_to_array(idx)) = op(tuple_to_array(idx));
    }
}

template<size_t N>
void assign_condition(Euler<N>& eq, auto rho_op, auto p_op, auto U_op){

    auto& prim = eq.primitive_variables();
    auto rho = make_full_span(prim.rho);
    auto p = make_full_span(prim.p);
    auto U = make_full_span(prim.U);

    for_each_index(rho, rho_op);
    for_each_index(p, p_op);
    for_each_index(U, U_op);
}


template<size_t Dir, size_t N>
void assign_shocktube(Euler<N>& eq){

    auto dims = full_extent(eq.primitive_variables().rho);

    auto rho_op = [=](auto i){
        if (std::get<Dir>(i) < size_t(0.5*dims.extent(Dir))){
            return 1.0;
        }
        return 0.125;
    };
    auto p_op = [=](auto i){
        if (std::get<Dir>(i) < size_t(0.5*dims.extent(Dir))){
            return 1.0;
        }
        return 0.1;
    };
    auto U_op = [=](auto i){
        (void) i;
        return Vector<N>{};
    };

    assign_condition(eq, rho_op, p_op, U_op);

}