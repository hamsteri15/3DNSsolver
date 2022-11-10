#pragma once

#include <array>

#include "jada/cartesian_product.hpp"
#include "jada/indices.hpp"
#include "jada/mdspan.hpp"
#include "jada/rank.hpp"

/// @brief Returns a view of multi-dimensional index tuples
/// @param begin index set of begin indices
/// @param end index set of end indices
/// @return A view of index tuples from [begin, end)
static constexpr auto md_indices(auto begin, auto end) {

    using Idx = typename decltype(begin)::value_type;

    return [&]<Idx... Is>(std::integer_sequence<Idx, Is...>) {
        return cartesian_product(indices(std::get<Is>(begin), std::get<Is>(end))...);
    }
    (std::make_integer_sequence<Idx, Idx(rank(begin))>{});
}

/// @brief Returns all multi-dimensional indices spanned by the extent of the input span
/// @param span the input span to query the extent from
/// @return A view of index tuples from [begin=0, extent(span) )
static constexpr auto all_indices(auto span) {
    using Idx = typename decltype(span)::index_type;
    return md_indices(std::array<Idx, rank(span)>{}, dimensions(span));
}
