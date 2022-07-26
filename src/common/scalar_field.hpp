#pragma once

#include <iostream>
#include <concepts>
#include "common/scalar_array.hpp"


using scalarField = scalar_array;

static inline void print(const scalarField& f){

    std::cout << "{" << std::endl;
    for (auto ff : f){
        std::cout << ff << std::endl;
    }
    std::cout << "}" << std::endl;

}


template<class T>
concept scalarField_c = std::same_as<T, scalarField>;
