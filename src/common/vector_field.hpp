#pragma once

#include <iostream>

#include "common/allocator.hpp"
#include "common/scalar.hpp"
#include "common/scalar_field.hpp"
#include "common/vector.hpp"
#include "topaz/include/all.hpp"

template<size_t L>
using vectorField = topaz::NumericArray<Vector<L>, vector_allocator_t<L>>;

template<size_t L>
static inline void print(const vectorField<L>& f){

    std::cout << "{" << std::endl;
    for (auto ff : f){
        std::cout << ff << std::endl;
    }
    std::cout << "}" << std::endl;

}