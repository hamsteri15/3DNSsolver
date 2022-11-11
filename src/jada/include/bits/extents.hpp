#pragma once

#include "rank.hpp"
#include "utils.hpp"
#include <array>
#include <experimental/mdspan>

namespace stdex = std::experimental;

template <size_t N> using extents = stdex::dextents<size_t, N>;

/// @brief Converts array-like dimensions to extents
/// @param dims array-like object with spatial dimensions
/// @return extents object with the same dimensions
static constexpr auto make_extent(auto dims) { return extents<jada::rank(dims)>{dims}; }

/// @brief Converts extents to std::array
/// @param ext extensions to convert
/// @return std::array<size_t, Rank> array with the extents
static constexpr auto jada::extent_to_array(auto ext) {

    using Idx = typename decltype(ext)::index_type;

    auto f = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::array<Idx, sizeof...(Is)>{ext.extent(Is)...};
    };
    return f(std::make_index_sequence<jada::rank(ext)>{});
}

/// @brief Computes the total element count spanned by the extents
/// @param ext extensions to compute the flat size of
/// @return size_t the flat size
static constexpr size_t flat_size(auto ext) {

    size_t ret(1);
    for (size_t i = 0; i < jada::rank(ext); ++i) { ret *= ext.extent(i); }
    return ret;
}

/// @brief Ensures that the input indices are all smaller than the input dims
/// @param indices set of indices (array/tuple) corresponding to a spatial location
/// @param dims input extensions
/// @return true if all indices in bounds, false otherwise
static constexpr bool indices_in_bounds(auto indices, auto dims) {

    auto extents = make_extent(dims);
    
    auto f = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::array<bool, sizeof...(Is)>{std::get<Is>(indices) < extents.extent(Is)...};
    };

    auto arr = f(std::make_index_sequence<jada::rank(extents)>{});

    return std::ranges::all_of(arr, [](bool b) { return b == true; });
}
