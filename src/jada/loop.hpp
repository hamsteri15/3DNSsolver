#pragma once

#include <array>

#include "jada/indices.hpp"
#include "jada/cartesian_product.hpp"
#include "jada/mdspan.hpp"
#include "jada/rank.hpp"


auto md_indices(auto begin, auto end){

    static constexpr size_t N = std::size(begin);
    using Idx = typename decltype(begin)::value_type;

    return [&]<Idx... Is>(std::integer_sequence<Idx, Is...>) {
        return cartesian_product(
            indices(begin[Is], end[Is])...
        );
    }(std::make_integer_sequence<Idx, Idx(N)>{});

}



auto all_indices(auto span){

    //const auto ext = s.extents();
    using idx_t = typename decltype(span)::index_type;

    std::array<idx_t, rank(span)> begin{};
    auto end = extent_to_array(span.extents());
    return md_indices(begin, end);
}

