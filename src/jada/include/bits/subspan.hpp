#pragma once

#include "mdspan.hpp"
#include "rank.hpp"
#include <array>

namespace detail {

// Dir != I -> return a
template <size_t Dir, size_t I> struct SubspanParams {
    static constexpr auto process(auto a, auto b) {
        (void)b;
        return std::get<I>(a);
    }
};

// Dir == I -> return a pair from a to b
template <size_t K> struct SubspanParams<K, K> {
    static constexpr auto process(auto a, auto b) {
        return std::make_pair(std::get<K>(a), std::get<K>(b));
    }
};

template <size_t Dir, class Arr> static constexpr auto make_tiled_subspan_params(Arr a, Arr b) {

    return [=]<size_t... Is>(std::index_sequence<Is...>) {
        return std::make_tuple(SubspanParams<Dir, Is>::process(a, b)...);
    }
    (std::make_index_sequence<jada::rank(a)>{});
}

template <size_t Dir> static constexpr auto make_tiled_subspan(auto s, auto begin, auto end) {

    auto callable = [=](auto... params) constexpr { return stdex::submdspan(s, params...); };

    return std::apply(callable, detail::make_tiled_subspan_params<Dir>(begin, end));
}

} // namespace detail

/// @brief Creates a subspan from the input span based on the index sets begin and end
/// @param span the input span to create the subspan from
/// @param begin set of indices describing the beginning of the subpan
/// @param end set of indices describing the end of the subspan
/// @return a subspan form [begin, end)
static constexpr auto jada::make_subspan(auto span, auto begin, auto end) {

    static_assert(jada::rank(span) == jada::rank(begin), "Dimension mismatch in jada::make_subspan");
    static_assert(jada::rank(begin) == jada::rank(end), "Dimension mismatch in jada::make_subspan");

    auto tpl = [=]<size_t... Is>(std::index_sequence<Is...>) constexpr {
        return std::make_tuple(span, std::make_pair(std::get<Is>(begin), std::get<Is>(end))...);
    }
    (std::make_index_sequence<jada::rank(span)>{});

    auto callable = [](auto... params) { return stdex::submdspan(params...); };

    return std::apply(callable, tpl);
}

/// @brief Creates a one-dimensional subspan along the index Dir based on the index set begin and an
/// integer extent
/// @tparam Dir the directional index to which the create the subspan along. If the mapping is such
/// that dimensions are [nz, ny, nx], Dir = 1 would create a subspan along y-direction
/// @param s the input span to create the subspan from
/// @param begin a set of indices describing the beginning of the subspan
/// @param extent width of the one-dimensional subspan
/// @return a one-dimensional subspan along the direction Dir
template <size_t Dir> static constexpr auto make_tiled_subspan(auto s, auto begin, size_t extent) {

    auto end(begin);
    std::get<Dir>(end) += extent;
    return detail::make_tiled_subspan<Dir>(s, begin, end);
}

/// @brief Creates a one-dimensional non-bounded subspan along the index Dir based on the index set
/// center
/// @tparam Dir the directional index to which the create the subspan along. If the mapping is such
/// that dimensions are [nz, ny, nx], Dir = 1 would create a subspan along y-direction
/// @param s the input span to create the subspan from
/// @param center a set of indices describing the center of the subspan
/// @return a one-dimensional subspan along the direction Dir. If the input center is [1,2,3] and
/// Dir=1 the returned subspan(-1) gives values at [1,1,3] and subspan(+1) gives values at [1,3,3].
template <size_t Dir> static inline auto make_tiled_subspan(auto s, auto center) {

    return detail::make_tiled_subspan<Dir>(s, center, center);
}