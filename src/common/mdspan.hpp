#pragma once


#include <experimental/mdspan>
#include "math.hpp"
#include "index_type.hpp"



namespace stdex = std::experimental;

using layout = stdex::layout_right;

template<size_t N>
using extents = stdex::dextents<size_t, N>;

template<class ElementType, size_t N>
using span = stdex::mdspan<ElementType, extents<N>, layout>;


//using extents = stdex::extents

template<class Field, class Extents>
static inline auto make_span(Field& field, Extents dims){
    return stdex::mdspan{field.data(), dims};
}
template<class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims){
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
