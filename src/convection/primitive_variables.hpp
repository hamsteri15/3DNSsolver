#pragma once

#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"

template <size_t N> struct PrimitiveVariables {

    PrimitiveVariables(size_t size)
        : rho(size)
        , p(size)
        , U(size) {}

    scalarField    rho;
    scalarField    p;
    vectorField<N> U;
};
