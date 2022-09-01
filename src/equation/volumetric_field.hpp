#pragma once

#include "common/field.hpp"
#include "equation/cartesian_grid.hpp"

template <class ET, size_t N> struct VolumetricField : public Field<ET> {

    using base_type = Field<ET>;
    using base_type::operator=;

    VolumetricField(const CartesianGrid<N>& grid)
        : Field<ET>(flat_size(grid.dimensions()))
        , m_grid(grid) {}


    /*
    auto internalField() {



    }
    */



    const auto& get_grid() const { return m_grid; }
    auto&       get_grid() { return m_grid; }

private:
    CartesianGrid<N> m_grid;
};