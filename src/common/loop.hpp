#pragma once

#include <array>

#include <range/v3/view/indices.hpp>
#include <range/v3/view/cartesian_product.hpp>

#include "cartesian_product.hpp"

#include "common/mdspan.hpp"



template<size_t N, class Idx>
auto md_indices(std::array<Idx, N> begin, std::array<Idx, N> end){

    return [&]<Idx... Is>(std::integer_sequence<Idx, Is...>) {
        return cartesian_product(
            ranges::views::indices(begin[Is], end[Is])...
        );
    }(std::make_integer_sequence<Idx, Idx(N)>{});

}


auto all_indices(auto s){

    //const auto ext = s.extents();
    static constexpr size_t N = decltype(s)::rank();

    using idx_t = typename decltype(s)::index_type;

    std::array<idx_t, N> begin{};
    auto end = extent_to_array(s.extents());
    return md_indices(begin, end);
}

