#include "diffusion_scheme.hpp"

double DiffusionScheme::tauxx(double mu, double du_dx, double dv_dy, double lamb) {

    return 2.0 * mu * du_dx + lamb * (du_dx + dv_dy);
}

double DiffusionScheme::tauyy(double mu, double du_dx, double dv_dy, double lamb) {

    return 2.0 * mu * dv_dy + lamb * (du_dx + dv_dy);
}

double DiffusionScheme::tauxy(double mu, double du_dy, double dv_dx) {

    return mu * (du_dy + dv_dx);
}

double DiffusionScheme::dtauxx_dx(double mu,
                                  double dmu_dx,
                                  double du_dx,
                                  double dv_dy,
                                  double du_dxdx,
                                  double dv_dydx,
                                  double lamb,
                                  double dlamb_dx) {

    double ret = 2.0 * dmu_dx * du_dx + 2.0 * mu * du_dxdx + dlamb_dx * (du_dx + dv_dy);
    ret += lamb * (du_dxdx + dv_dydx);
    return ret;
}

double DiffusionScheme::dtauyy_dy(double mu,
                                  double dmu_dy,
                                  double dv_dy,
                                  double dv_dydy,
                                  double du_dx,
                                  double du_dxdy,
                                  double lamb,
                                  double dlamb_dy) {

    double ret = 2.0 * dmu_dy * dv_dy + 2.0 * mu * dv_dydy + dlamb_dy * (du_dx + dv_dy);
    ret += lamb * (du_dxdy + dv_dydy);
    return ret;
}

double DiffusionScheme::dtauxy_dx(
    double mu, double dmu_dx, double du_dy, double dv_dx, double du_dydx, double dv_dxdx) {

    double ret = dmu_dx * (du_dy + dv_dx) + mu * (du_dydx + dv_dxdx);
    return ret;
}

double DiffusionScheme::dtauyx_dy(
    double mu, double dmu_dy, double du_dy, double dv_dx, double du_dydy, double dv_dxdy) {

    double ret = dmu_dy * (du_dy + dv_dx) + mu * (du_dydy + dv_dxdy);
    return ret;
}

Array4d DiffusionScheme::divide_p_by_rho(const Array4d& primitive) {

    Array4d ret = Array4d(primitive.get_dimensions());

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = primitive.I_BEGIN(); i < primitive.I_END(); i++) {
            for (size_t j = primitive.J_BEGIN(); j < primitive.J_END(); j++) {
                for (size_t k = primitive.K_BEGIN(); k < primitive.K_END(); k++) {

                    ret(i, j, k, 0) = 0.0; // arbitrary, no diffusion component in continuity eq.
                    ret(i, j, k, 1) = primitive(i, j, k, 1) / primitive(i, j, k, 0); // p/rho
                    ret(i, j, k, 2) = primitive(i, j, k, 2);                         // u
                    ret(i, j, k, 3) = primitive(i, j, k, 3);                         // v
                    ret(i, j, k, 4) = primitive(i, j, k, 4);                         // w
                }
            }
        }

    } // pragma

    return ret;
}
