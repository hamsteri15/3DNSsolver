#pragma once


#include <experimental/mdspan>
#include "common/math.hpp"
#include "common/index_type.hpp"
#include "common/runtime_assert.hpp"



namespace stdex = std::experimental;

using layout = stdex::layout_right;

template<size_t N>
using extents = stdex::dextents<size_t, N>;

template<class ElementType, size_t N>
using span = stdex::mdspan<ElementType, extents<N>, layout>;


auto extent_to_array(auto ext){

    static constexpr size_t N = decltype(ext)::rank();
    using idx_t = typename decltype(ext)::index_type;

    std::array<idx_t, N> ret{};
    for (size_t i = 0; i < N; ++i){
        ret[i] = ext.extent(i);
    }

    return ret;
}


template<class Extents>
bool extents_equal_size(size_t size, Extents dims){

    auto arr = extent_to_array(dims);
    using idx_t = typename decltype(arr)::value_type;
    auto mult = std::accumulate(arr.begin(), arr.end(), idx_t(1), std::multiplies{});

    return size_t(mult) == size;

}


template<class Field, class Extents>
static inline auto make_span(Field& field, Extents dims){
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return stdex::mdspan{field.data(), dims};
}
template<class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims){
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return stdex::mdspan{field.data(), dims};
}

template<class Span>
static constexpr size_t rank(const Span& span){
    return span.rank();
}




/*
template<class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims){

    return stdex::mdspan{field.data(), dims};

}
*/
