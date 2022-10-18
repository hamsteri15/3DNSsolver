#pragma once

#include "common/mdspan.hpp"
#include <array>

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

namespace detail {

template <size_t Dir, size_t I> struct SubspanParams {
    static constexpr auto process(auto a, auto b) {
        (void)b;
        return std::get<I>(a);
    }
};

template <size_t K> struct SubspanParams<K, K> {
    static constexpr auto process(auto a, auto b) {
        return std::make_pair(std::get<K>(a), std::get<K>(b));
    }
};

template <size_t Dir, size_t N>
auto make_tiled_subspan_params(std::array<size_t, N> a, std::array<size_t, N> b) {

    auto make_tuple = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(SubspanParams<Dir, Is>::process(a, b)...);
    };

    return make_tuple(std::make_index_sequence<N>());
}

} // namespace detail

template <size_t Dir, size_t N, class T>
static inline auto
make_tiled_subspan(T s, std::array<size_t, N> center, size_t half_width) {
    std::array<size_t, N> begin(center);
    std::array<size_t, N> end(center);
    begin[Dir] -= half_width;
    end[Dir] += half_width;

    auto tpl = detail::make_tiled_subspan_params<Dir>(begin, end);

    auto callable = [=](auto... params) { return stdex::submdspan(s, params...); };

    return std::apply(callable, tpl);
}
