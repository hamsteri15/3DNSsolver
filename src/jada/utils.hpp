#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <stdexcept> //std::runtime_error
#include <string>

//Trait to type check all types of a std::tuple
template<template<typename, typename...> class Trait, typename Tuple>
struct all_of;

template<template<typename, typename...> class Trait, typename... Types>
struct all_of<Trait, std::tuple<Types...>> : std::conjunction<Trait<Types>...> {};



#ifdef NDEBUG
constexpr void runtime_assert([[maybe_unused]] bool condition, [[maybe_unused]] const char* msg) {}
#else
constexpr void runtime_assert(bool condition, const char* msg) {
    if (!condition) throw std::runtime_error(msg);
}
#endif


/// @brief Converts all tuple elements to an array of elements of the same size
/// @tparam tuple_t the type of the tuple to convert
/// @param tuple the input tuple to convert
/// @return std::array of the tuple elements
template <typename tuple_t> constexpr auto tuple_to_array(tuple_t&& tuple) {
    
    constexpr auto get_array = [](auto&&... x) constexpr {
        return std::array{std::forward<decltype(x)>(x)...};
    };
    return std::apply(get_array, std::forward<tuple_t>(tuple));
}
