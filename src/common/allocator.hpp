#pragma once

#include "common/scalar.hpp"
#include "common/vector.hpp"

#ifdef __CUDACC__
#include <thrust/device_vector.h>
#include <thrust/device_malloc_allocator.h>
template<class T>
using allocator_t = thrust::device_malloc_allocator<T>;


#else

#include <vector>
template<class T>
using allocator_t = std::allocator<T>;

#endif


using scalar_allocator_t = allocator_t<scalar>;

template<size_t L>
using vector_allocator_t = allocator_t<Vector<L>>;