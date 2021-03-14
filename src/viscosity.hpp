#ifndef Viscosity_hh
#define Viscosity_hh
#include <cmath> //sqrt

#include "./params.hpp"
#include "containers/array_3d.hpp"
#include "containers/array_4d.hpp"

class Viscosity {

public:
    Viscosity() = default;

    ~Viscosity() = default;

    Array3d get_viscosity(const Array4d& primitive);

    inline double mu_sutherland(double T, double mu0, double T0, double C);

private:
    // double m_mu_zero;
};

#endif