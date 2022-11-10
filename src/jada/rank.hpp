#pragma once

#include <array>
#include <cstddef>

template<class T>
struct Rank{
    static constexpr size_t value = T::rank(); 
};

template<size_t N>
struct Rank<std::array<size_t, N>>{
    static constexpr size_t value = N;
};
