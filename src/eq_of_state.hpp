#ifndef EqOfState_hh
#define EqOfSTate_hh
#include <cmath> //sqrt

#include "containers/array_4d.hpp"
#include "params.hpp"

class EqOfState {

public:
    EqOfState() = default;

    ~EqOfState() = default;

    EqOfState(double gamma)
        : m_gamma(gamma) {}

    EqOfState(const Params& p)
        : m_gamma(p.gamma) {}

    void primitive_to_conservative(const Array4d& W, Array4d& U);

    void conservative_to_primitive(const Array4d& U, Array4d& W);

    // returns the total energy
    double E(double rho, double p, double u, double v, double w);

    // returns the maximum local time step based on wave speeds
    double
    local_dt(double rho, double p, double u, double v, double w, double dx, double dy, double dz);

private:
    double m_gamma;
};

#endif