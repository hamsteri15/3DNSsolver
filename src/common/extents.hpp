#pragma once

#include <experimental/mdspan>


namespace stdex = std::experimental;


template<size_t N>
using extents = stdex::dextents<size_t, N>;
