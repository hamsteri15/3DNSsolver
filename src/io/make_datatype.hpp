#pragma once

#include <h5wrapper.hpp>
#include "math/math_vector.hpp"


namespace H5Wrapper{


template <class T, size_t L> 
struct H5DatatypeCreator<MathVector<T, L>> {

    static H5Datatype create() {
        auto element_type = H5DatatypeCreator<T>::create();
        return H5DatatypeArray(element_type, 1, std::vector<size_t>{L});
    }
};


}
