#pragma once

#include "H5Wrapper/include/h5_datatype_creator.hpp"
#include "common/math_vector_base.hpp"


namespace H5Wrapper{


template <class T, size_t L, class S> 
struct H5DatatypeCreator<MathVectorBase<T, L, S>> {

    static H5Datatype create() { return H5Datatype::copy(H5T_NATIVE_DOUBLE); }
};


}
