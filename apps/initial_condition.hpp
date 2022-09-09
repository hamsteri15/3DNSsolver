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

        if (std::get<Dir>(idx) < size_t(0.5*dims.extent(Dir))){

            rho(idx) = 1.0;
            p(idx) = 1.0;
            U(idx) = Vector<N>{};
        }
        else{
            rho(idx) = 0.125;
            p(idx) = 0.1;
            U(idx) = Vector<N>{};
        }

    }
}