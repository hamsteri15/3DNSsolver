#pragma once

#include <concepts>

using scalar = double;

template<class T>
concept scalar_c = std::same_as<T, scalar>; 

