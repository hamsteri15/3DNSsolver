#ifndef Weno5_hh
#define Weno5_hh

#include "convection_schemes/convection_scheme.hpp"
#include <iostream>

class Weno5 : public ConvectionScheme {

public:
    Weno5()
        : ConvectionScheme() {}

    Weno5(std::string type, double gamma, std::string weight_type)
        : ConvectionScheme(type, gamma)
        , m_weight_type(weight_type) {}

    virtual ~Weno5() {}

    virtual void solve(const Grid&    grid,
                       const Array4d& primitive,
                       const Array4d& conservative,
                       Array4d&       residual);

protected:
    std::string m_weight_type;

    double get_max_alpha(double d1, double d2, double d3, double d4, double d5);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Computes the weights for the given
    ///		\brief			five point stencil according to Shu et al.
    ///
    ///		\param f_m2	f_{i - 2}
    ///		\param f_m1	f_{i - 1}
    ///		\param f_	f_{i}
    ///		\param f_p1	f_{i + 1}
    ///		\param f_p2	f_{i + 2}
    ///		\param d	constants for the weight calculation
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    std::array<T, 3> WENO5_weights_shu(T f_m2, T f_m1, T f_, T f_p1, T f_p2, const double d[3]);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Computes the weights for the given
    ///		\brief			five point stencil according to Borges et al.
    ///
    ///		\param f_m2	f_{i - 2}
    ///		\param f_m1	f_{i - 1}
    ///		\param f_	f_{i}
    ///		\param f_p1	f_{i + 1}
    ///		\param f_p2	f_{i + 2}
    ///		\param d	constants for the weight calculation
    ///
    ////////////////////////////////////////////////////////////
    template <typename T>
    std::array<T, 3> WENO5_weights_borges(T f_m2, T f_m1, T f_, T f_p1, T f_p2, const double d[3]);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Computes the WENO-5 numerical flux
    ///		\brief			for left biased stencil
    ///
    ///		\param fl_m2	f^L_{i,j - 2}
    ///		\param fl_m1	f^L_{i,j - 1}
    ///		\param fl_		f^L_{i,j 	}
    ///		\param fl_p1	f^L_{i,j + 1}
    ///		\param fl_p2	f^L_{i,j + 2}
    ///
    ////////////////////////////////////////////////////////////
    template <typename T> T WENO5_left(T fl_m2, T fl_m1, T fl_, T fl_p1, T fl_p2);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Computes the WENO-5 numerical flux
    ///		\brief			for right biased stencil
    ///
    ///		\param fr_m2	f^R_{i,j - 2}
    ///		\param fr_m1	f^R_{i,j - 1}
    ///		\param fr_		f^R_{i,j 	}
    ///		\param fr_p1	f^R_{i,j + 1}
    ///		\param fr_p2	f^R_{i,j + 2}
    ///
    ////////////////////////////////////////////////////////////
    template <typename T> T WENO5_right(T fR_m2, T fR_m1, T fR_, T fR_p1, T fR_p2);

private:
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

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief			Computes the flux differences f_x, f_y, f_z
    ///
    ///		\param			grid dx, dy, dz  info
    ///		\param			R Residual array
    ///		\param			f_x, f_y, f_z Weno numerical fluxes
    ///
    ////////////////////////////////////////////////////////////
    void compute_residual(const Grid&    grid,
                          const Array4d& f_x,
                          const Array4d& f_y,
                          const Array4d& f_z,
                          Array4d&       residual);
};

#endif
