#pragma once

#include <array>
#include "common/mdspan.hpp"



template <class T, size_t N>
static inline auto
make_subspan(span<T, N> s, std::array<size_t, N> begin, std::array<size_t, N> end) {

    std::array<std::pair<size_t, size_t>, N> idx_pairs;
    for (size_t i = 0; i < N; ++i) { idx_pairs[i] = std::make_pair(begin[i], end[i]); }

    auto make_tuple = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(s, idx_pairs[Is]...);
    };

    auto tpl = make_tuple(std::make_index_sequence<N>());

    auto callable = [](auto... params) { return stdex::submdspan(params...); };

    return std::apply(callable, tpl);
}



/*
template <size_t Dir, class T, size_t N>
static inline auto
make_reduced_subspan(span<T, N> s, std::array<size_t, N> begin, size_t width) {





}
*/
