#pragma once

#include <concepts>

using scalar = double;

template<class T>
concept scalar_c = std::same_as<T, scalar>;


static constexpr scalar mag(const scalar& f){
    if (f < scalar(0)) {return -f;}
    return f;
}