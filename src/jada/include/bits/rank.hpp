#pragma once

#include <array>
#include <cstddef>
#include "utils.hpp"
#include "indices.hpp"


template <class T> struct Rank { static constexpr size_t value = T::jada::rank(); };

template <size_t N, class Idx> struct Rank<std::array<Idx, N>> {
    static_assert(std::is_integral<Idx>::value, "Not an integer array.");
    static constexpr size_t value = N;
};

template <class... Idx> struct Rank<std::tuple<Idx...>> {

private:
    using my_type = std::tuple<Idx...>;
    
public:    
    static_assert(all_of<std::is_integral, my_type>::value, "Not an integer tuple.");
    static constexpr size_t value = sizeof...(Idx);
};

template <class... Idx> struct Rank<ranges::common_tuple<Idx...>> {

private:
    using my_type = std::tuple<Idx...>;
    
public:    
    //static_assert(all_of<std::is_integral, my_type>::value, "Not an integer tuple.");
    static constexpr size_t value = sizeof...(Idx);
};



/// @brief Queries the _static_ rank of the input type T
/// @tparam T the type to query the static rank of_
/// @return static rank of the input type T
template <class T> consteval size_t jada::rank(const T&) { return Rank<T>::value; }