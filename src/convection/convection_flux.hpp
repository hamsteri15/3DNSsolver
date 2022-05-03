#pragma once

#include "common/scalar_soa.hpp"



struct ConvectionFlux1D : public scalar_soa<3>
{
    enum class Components{
        cont = size_t(0),
        x_mom = size_t(1)
    };

    //ConvectionFlux1D(size_t ads) {}

};

struct ConvectionFlux2D{};

struct ConvectionFlux3D{};
