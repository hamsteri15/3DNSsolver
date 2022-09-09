#pragma once

#include "common/mdspan.hpp"
#include "equation/euler.hpp"
#include "equation/volumetric_field.hpp"
#include "equation/boundary_condition.hpp"


template<size_t Dir, size_t N, class T>
void set_linear(span<T, N> s){

    auto indices = all_indices(s);
    for (auto idx : indices){
        auto ii = std::get<Dir>(idx);
        s(idx) = T(ii);
    }

}
template<size_t N>
auto make_euler_equation(extents<N> dims, extents<N> padding){

    Vector<N> p0;
    Vector<N> p1;
    p0 = 0.0;
    p1 = 1.0;
    CartesianGrid<N> grid(dims, p0, p1);
    return Euler(grid, padding, EquationOfState{});
}

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

template<size_t N>
void mirror_all(Euler<N>& eq){


    auto& p = eq.primitive_variables();

    for (size_t i = 0; i < N; ++i){

        Vector<N> normal{};
        normal[i] = 1;
        mirror(p.rho, normal);
        mirror(p.p, normal);
        mirror(p.U, normal);

        normal[i] = -1;
        mirror(p.rho, normal);
        mirror(p.p, normal);
        mirror(p.U, normal);

    }

}
