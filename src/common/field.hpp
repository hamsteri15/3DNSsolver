#pragma once

#include <concepts>

template<class T>
concept field_c = topaz::IsRangeOrNumericArray_v<T>;