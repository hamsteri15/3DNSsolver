#pragma once

#include "jada/mdspan.hpp"
#include "jada/rank.hpp"
#include <array>

static inline auto make_subspan(auto span, auto begin, auto end) {

    static constexpr size_t N = Rank<decltype(begin)>::value;
    static_assert(N == Rank<decltype(end)>::value, "Dimension mismatch in make_subspan");
    static_assert(Rank<decltype(span)>::value == N, "Dimension mismatch in make_subspan");

    std::array<std::pair<size_t, size_t>, N> idx_pairs;
    for (size_t i = 0; i < N; ++i) { idx_pairs[i] = std::make_pair(begin[i], end[i]); }

    auto make_tuple = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(span, idx_pairs[Is]...);
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

template <size_t Dir, class Arr> auto make_tiled_subspan_params(Arr a, Arr b) {

    static constexpr auto N = std::size(a);

    auto make_tuple = [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(SubspanParams<Dir, Is>::process(a, b)...);
    };

    return make_tuple(std::make_index_sequence<N>());
}

} // namespace detail

template <size_t Dir>
static inline auto make_tiled_subspan(auto s, auto begin, auto end) {

    auto tpl = detail::make_tiled_subspan_params<Dir>(begin, end);

    auto callable = [=](auto... params) { return stdex::submdspan(s, params...); };

    return std::apply(callable, tpl);
}

template <size_t Dir>
static inline auto make_tiled_subspan(auto s, auto center) {

    return make_tiled_subspan<Dir>(s, center, center);
}