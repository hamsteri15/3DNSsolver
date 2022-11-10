#pragma once
#include <experimental/mdspan>
#include <range/v3/view/indices.hpp>
#include <iostream>
#include "jada/extents.hpp"
#include "jada/utils.hpp"

namespace stdex = std::experimental;


template <class ElementType, size_t N, class Layout>
using span_base = stdex::mdspan<ElementType, extents<N>, Layout>;

template<class ElementType, size_t N>
using span = span_base<ElementType, N, stdex::layout_right>;

template<class Field>
static inline auto make_span(Field& field, auto dims) {
    using value_type          = typename Field::value_type;
    auto ext = make_extent(dims);
    static constexpr size_t N = decltype(ext)::rank();
    runtime_assert(flat_size(dims) == field.size(), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), ext);
}
template<class Field>
static inline auto make_span(const Field& field, auto dims) {
    using value_type          = const typename Field::value_type;
    auto ext = make_extent(dims);
    static constexpr size_t N = decltype(ext)::rank();
    runtime_assert(flat_size(dims) == field.size(), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), ext);
}




template <class Span> void print(Span span) {

    if constexpr(rank(span)==1){

        for (size_t i = 0; i < span.extent(0); ++i) { std::cout << span(i) << " "; }
        std::cout << std::endl;
    }
    else if constexpr (rank(span) == 2){
        for (size_t i = 0; i < span.extent(0); ++i) {
            for (size_t j = 0; j < span.extent(1); ++j) { std::cout << span(i, j) << " "; }
            std::cout << std::endl;
        }
    }
    else{
        throw std::logic_error("Only ranks 1 and 2 spans can be printed");
    }
}

