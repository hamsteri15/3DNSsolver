#pragma once

#include <array>

#include "common/allocator.hpp"
#include "common/scalar.hpp"
#include "common/scalar_field.hpp"
#include "common/vector.hpp"
#include "topaz/include/all.hpp"

template<size_t L>
using vectorField = topaz::NumericArray<Vector<L>, vector_allocator_t<L>>;
