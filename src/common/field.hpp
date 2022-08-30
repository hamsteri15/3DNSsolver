#pragma once

#include <concepts>
#include "common/allocator.hpp"
#include "topaz/include/all.hpp"

template<class ET>
using Field = topaz::NumericArray<ET, allocator_t<ET>>;


template<class T>
concept field_c = topaz::IsRangeOrNumericArray_v<T>;