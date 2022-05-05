#pragma once

#include <concepts>



template<class T>
concept Expression_c = topaz::IsRangeOrNumericVector_v<T>;