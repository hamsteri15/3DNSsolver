#pragma once

#include <iostream>
#include "math/vector.hpp"


template<size_t N>
struct CoordinateSystem{

    static constexpr Vector<N> origin{};

};

enum class CartesianAxis : size_t{
    X = 0,
    Y = 1,
    Z = 2
};

static std::string axis_name(CartesianAxis ax){

    switch(ax){
        case CartesianAxis::X:
            return "X";
        case CartesianAxis::Y:
            return "Y";
        case CartesianAxis::Z:
            return "Z";
        default:
            throw std::logic_error("Invalid Cartesian axis.");
    }

}



template<size_t N>
struct Cartesian : public CoordinateSystem<N>{


    static constexpr Vector<N> unit_normal(CartesianAxis ax){

        Vector<N> v{};
        v[size_t(ax)] = 1.0;
        return v;
    }

    static constexpr size_t axis_index(CartesianAxis ax){
        switch(ax){
            case CartesianAxis::X:
                return size_t(1);
            case CartesianAxis::Y:
                return size_t(2);
            case CartesianAxis::Z:
                return size_t(3);
            default:
                throw std::logic_error("Invalid Cartesian axis.");
        }

    }

    static size_t num_spatial_dims() {return N;}

};

