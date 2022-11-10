#pragma once
#include "jada/extents.hpp"
#include "jada/utils.hpp"
#include <experimental/mdspan>
#include <iostream>
#include <range/v3/view/indices.hpp>

namespace stdex = std::experimental;

template <class ElementType, size_t N, class Layout>
using span_base = stdex::mdspan<ElementType, extents<N>, Layout>;

template <class ElementType, size_t N> using span = span_base<ElementType, N, stdex::layout_right>;

//TODO: rename this function as extents and rename the extents object something else
/// @brief Returns the extent of the input span
/// @tparam Span a mdspan type
/// @param span the input span to query the extent of
/// @return extent of the input span
template <class Span> static constexpr auto extent(const Span& span) { return span.extents(); }

/// @brief Returns the dimensions of the input span as an std::array
/// @tparam Span a mdspan type
/// @param span the input span to query the dimensions of
/// @return std::array of dimensions spanned by the input span
template <class Span> static constexpr auto dimensions(const Span& span) {
    return extent_to_array(extent(span));
}

/// @brief Makes a multi-dimensional span of the input container
/// @tparam Container a container which has a value_type, size() and data() members
/// @param c the input container
/// @param dims dimensions of the multi-dimensional span
/// @return a multi-dimensional span
template <class Container> static constexpr auto make_span(Container& c, auto dims) {
    using value_type = typename Container::value_type;
    auto ext         = make_extent(dims);
    runtime_assert(flat_size(dims) == std::size(c), "Dimension mismatch in make_span");
    return span<value_type, rank(ext)>(std::data(c), ext);
}

/// @brief Makes a multi-dimensional span of the input container
/// @tparam Container a container which has a value_type, size() and data() members
/// @param c the input container
/// @param dims dimensions of the multi-dimensional span
/// @return a multi-dimensional span
template <class Container> static constexpr auto make_span(const Container& c, auto dims) {
    using value_type = const typename Container::value_type;
    auto ext         = make_extent(dims);
    runtime_assert(flat_size(dims) == std::size(c), "Dimension mismatch in make_span");
    return span<value_type, rank(ext)>(std::data(c), ext);
}

// TODO: remove
template <class Span> void print(Span span) {

    if constexpr (rank(span) == 1) {

        for (size_t i = 0; i < span.extent(0); ++i) { std::cout << span(i) << " "; }
        std::cout << std::endl;
    } else if constexpr (rank(span) == 2) {
        for (size_t i = 0; i < span.extent(0); ++i) {
            for (size_t j = 0; j < span.extent(1); ++j) { std::cout << span(i, j) << " "; }
            std::cout << std::endl;
        }
    } else {
        throw std::logic_error("Only ranks 1 and 2 spans can be printed");
    }
}
