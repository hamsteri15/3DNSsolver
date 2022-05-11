#pragma once

#include <concepts>



template<class T>
concept Expression_c = topaz::IsRangeOrNumericArray_v<T>;