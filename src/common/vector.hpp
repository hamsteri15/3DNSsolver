#pragma once

#include <concepts>

#include "common/scalar.hpp"
#include "common/math_vector.hpp"


template<size_t L>
using Vector = MathVector<scalar, L>;

template<class T, size_t L>
concept vector_c = std::same_as<T, Vector<L>>;