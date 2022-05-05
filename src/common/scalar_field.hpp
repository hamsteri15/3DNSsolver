#pragma once

#include <concepts>
#include "common/scalar_array.hpp"


using scalarField = scalar_array;


template<class T>
concept scalarField_c = std::same_as<T, scalarField>;
