#ifndef DiffusionScheme_hh
#define DiffusionScheme_hh

#include "communicator.hpp"
#include "containers/array_4d.hpp"
#include "grid.hpp"
#include "io/writer.hpp" //debugging
#include "viscosity.hpp"
#include <iostream>

class DiffusionScheme {

public:
    virtual void solve(const Grid&         grid,
                       const Communicator& communicator,
                       const Array4d&      primitive,
                       Array4d&            residual) {}

    DiffusionScheme() = default;

    DiffusionScheme(std::string type, double gamma, double mu, double Pr)
        : m_type(type)
        , m_gamma(gamma)
        , m_mu(mu)
        , m_Pr(Pr) {}

    virtual ~DiffusionScheme() = default;

protected:
    std::string m_type;

    double m_gamma;
    double m_mu;
    double m_Pr;

    Array4d divide_p_by_rho(const Array4d& primitive);

    double tauxx(double mu, double du_dx, double dv_dy, double lamb);
    double tauyy(double mu, double du_dx, double dv_dy, double lamb);
    double tauxy(double mu, double du_dy, double dv_dx);

    double dtauxx_dx(double mu,
                     double dmu_dx,
                     double du_dx,
                     double dv_dy,
                     double du_dxdx,
                     double dv_dydx,
                     double lamb,
                     double dlamb_dx);
    double dtauyy_dy(double mu,
                     double dmu_dy,
                     double dv_dy,
                     double dv_dydy,
                     double du_dx,
                     double du_dxdy,
                     double lamb,
                     double dlamb_dy);
    double
    dtauxy_dx(double mu, double dmu_dx, double du_dy, double dv_dx, double du_dydx, double dv_dxdx);
    double
    dtauyx_dy(double mu, double dmu_dy, double du_dy, double dv_dx, double du_dydy, double dv_dxdy);
};

#endif