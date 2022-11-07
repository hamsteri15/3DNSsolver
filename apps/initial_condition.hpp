#pragma once

#include "equation/euler.hpp"



template<class ET, size_t N, class Op>
void for_all_coordinates(VolumetricField<ET, N>& f, Op op){

    auto span = make_internal_span(f);

    for (auto idx : all_indices(span)){
        span(tuple_to_array(idx)) = op(index_to_cell_center(f.grid(), idx));
    }

}

template<size_t Dir, size_t N>
void assign_shocktube(Euler<N>& eq){

    auto L = eq.grid().p1() - eq.grid().p0();

    auto rho_op = [=](auto x){
        if (x[Dir] < 0.5*L[Dir]){
            return 1.0;
        }
        return 0.125;
    };
    
    auto p_op = [=](auto x){
        if (x[Dir] < 0.5*L[Dir]){
            return 1.0;
        }
        return 0.1;
    };

    auto U_op = [=](auto x){
        (void) x;
        return Vector<N>{};
    };

   for_all_coordinates(eq.primitive_variables().rho, rho_op); 
   for_all_coordinates(eq.primitive_variables().p, p_op); 
   for_all_coordinates(eq.primitive_variables().U, U_op); 


}