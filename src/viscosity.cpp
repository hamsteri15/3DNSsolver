#include "viscosity.hpp"

Array3d Viscosity::get_viscosity(const Array4d& primitive){






}


inline double Viscosity::mu_sutherland(double T, double mu0, double T0, double C){

    static const double three_per_two = 3./2.; 

    double mu = mu0 * (T0 + C) / (T + C) * pow(T/T0, three_per_two);

    return mu;




}