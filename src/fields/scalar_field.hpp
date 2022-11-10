#pragma once

#include <iostream>
#include "math/scalar.hpp"
#include "fields/field.hpp"

using scalarField = Field<scalar>;

static inline void print(const scalarField& f){

    std::cout << "{" << std::endl;
    for (auto ff : f){
        std::cout << ff << std::endl;
    }
    std::cout << "}" << std::endl;

}


template<class T>
concept scalarField_c = std::same_as<T, scalarField>;
