#ifndef Weno5cw_hh
#define Weno5cw_hh

#include "convection_schemes/weno-5.hpp"
#include "matrix.hpp"
#include <iostream>

class Weno5cw : public Weno5 {

public:
    Weno5cw() = default;
    Weno5cw(std::string type, double gamma, std::string weight_type)
        : Weno5(type, gamma, weight_type) {}

    ~Weno5cw() = default;

protected:
private:
    void compute_num_flux(const double fL[5][5],
                          const double fR[5][5],
                          const double L[5][5],
                          const double R[5][5],
                          double*      flux);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief			Computes the Weno numerical fluxes
    ///
    ///		\param			F convection flux (at nodes)
    ///		\param			U conserved variables (at nodes)
    ///		\param			W primitive variables (at nodes)
    ///		\param			eig maximum eigenvalues (at nodes)
    ///		\param			f the numerical flux (at i+1/2)
    ///
    ////////////////////////////////////////////////////////////
    virtual void
    x_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f);
    virtual void
    y_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f);
    virtual void
    z_sweep(const Array4d& F, const Array4d& U, const Array4d& W, const Array3d& eig, Array4d& f);
};

#endif
