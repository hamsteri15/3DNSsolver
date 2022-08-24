#pragma once

#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"
#include "common/mdspan.hpp"
#include "differentiation/tiled_stencil.hpp"


template<class Ext, class... Ops>
scalarField make_scalar_field(Ext interior_dims, Ops... ops){

    auto padded_extent = make_padded_extent(interior_dims, ops...);
    return scalarField(flat_size(padded_extent));

}

template<size_t N, class Ext, class... Ops>
vectorField<N> make_vector_field(Ext interior_dims, Ops... ops){

    auto padded_extent = make_padded_extent(interior_dims, ops...);
    return vectorField<N>(flat_size(padded_extent));

}