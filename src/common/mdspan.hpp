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

template <class ElementType, size_t N, class Layout = stdex::layout_right>
struct span : public span_base<ElementType, N, Layout> {
    using parent = span_base<ElementType, N, Layout>;
    using parent::mdspan; // Inherit all constructors
    using parent::operator();

    template <class... Is> ElementType& operator()(ranges::common_tuple<Is...> tpl) {
        return parent::operator()(get_array_from_tuple(tpl));
    }

    template <class... Is> ElementType& operator()(ranges::common_tuple<Is...> tpl) const {
        return parent::operator()(get_array_from_tuple(tpl));
    }
};

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

