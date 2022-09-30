#pragma once

#include "equation/euler.hpp"

template<size_t Dir, size_t N>
void assign_shocktube(Euler<N>& eq){


    auto& prim = eq.primitive_variables();
    auto dims = full_extent(prim.rho);

    auto rho = make_full_span(prim.rho);
    auto p = make_full_span(prim.p);
    auto U = make_full_span(prim.U);

    for (auto idx : all_indices(rho)){

        auto ii = tuple_to_array(idx);
        
        if (std::get<Dir>(ii) < size_t(0.5*dims.extent(Dir))){
            
            rho(ii) = 1.0;
            p(ii) = 1.0;
            U(ii) = Vector<N>{};
        }
        else{
            rho(ii) = 0.125;
            p(ii) = 0.1;
            U(ii) = Vector<N>{};
        }

    }
}