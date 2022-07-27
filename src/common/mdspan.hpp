#pragma once


#include <experimental/mdspan>
#include "math.hpp"
#include "index_type.hpp"

namespace stdex = std::experimental;

//using extents = stdex::extents

template<class Field, class Extents>
static inline auto make_span(Field& field, Extents dims){

    return stdex::mdspan{field.data(), dims};

}
/*
template<class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims){

    return stdex::mdspan{field.data(), dims};

}
*/
