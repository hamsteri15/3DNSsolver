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

