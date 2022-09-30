#pragma once

#include "common/extents.hpp"
#include "common/index_type.hpp"
#include "common/math.hpp"
#include "common/runtime_assert.hpp"
#include "common/utils.hpp"
#include <experimental/mdspan>
#include <range/v3/view/indices.hpp>

namespace stdex = std::experimental;


template <class ElementType, size_t N, class Layout>
using span_base = stdex::mdspan<ElementType, extents<N>, Layout>;

template<class ElementType, size_t N>
using span = span_base<ElementType, N, stdex::layout_right>;



template <class Field, class Extents> static inline auto make_span(Field& field, Extents dims) {
    using value_type          = typename Field::value_type;
    static constexpr size_t N = Extents::rank();
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), dims);
}

template <class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims) {
    using value_type          = const typename Field::value_type;
    static constexpr size_t N = Extents::rank();
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), dims);
}


template <class Span> static constexpr size_t rank(const Span& span) { return span.rank(); }

template <size_t dir, index_type amount> auto shift(auto idx) {

    std::get<dir>(idx) += amount;
    return idx;
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

