#pragma once

#include "common/scalar.hpp"
#include "common/allocator.hpp"
#include "topaz/include/all.hpp"


template<size_t N>
using scalar_soa = topaz::NumericSoa<N, scalar, scalar_allocator_t>;

