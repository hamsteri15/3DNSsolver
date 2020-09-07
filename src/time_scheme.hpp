#ifndef timescheme_hh
#define timescheme_hh

#include "communicator.hpp"
#include "containers/array_4d.hpp"
#include "convection_schemes/convection_scheme.hpp"
#include "diffusion_schemes/diffusion_scheme.hpp"
#include "domain.hpp"
#include "eq_of_state.hpp"
#include "grid.hpp"
#include "timer.hpp"

class TimeScheme {

public:
    TimeScheme() = default;

    TimeScheme(const Params& p)
        : m_current_dt(0.0)
        , m_CFL_max(p.CFL_max)
        , m_eq_of_state(p.gamma) {}

    ~TimeScheme() {}

    void step(Domain&             domain,
              const Grid&         grid,
              const Communicator& communicator,
              ConvectionScheme*   convection,
              DiffusionScheme*    diffusion);

    double get_dt() const { return m_current_dt; }

private:
    double m_current_dt;
    double m_CFL_max;

    EqOfState m_eq_of_state;

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Returns the maximum time step for the
    ///    	\brief 			input primitive field and step sizes
    ///
    ///		\param primitive	primitive variables (rho, p, u, v, w)
    ///		\param Grid	    Grid info (dx, dy, dz)
    ///
    ////////////////////////////////////////////////////////////
    double get_local_max_dt(const Array4d& primitive, const Grid& grid);

    void set_dt(const Array4d& primitive, const Grid& grid);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Creates the proper convection solver
    ///		\brief			based on input params
    ///
    ///		\param p		Parameters of the case
    ///		\param m		Domain instance of the case
    ///
    ////////////////////////////////////////////////////////////
    // void create_convection_solver(const Params& p, Domain const* d);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Creates the proper diffusion solver
    ///		\brief			based on input params
    ///
    ///		\param p		Parameters of the case
    ///		\param m		Domain instance of the case
    ///
    ////////////////////////////////////////////////////////////
    // void create_diffusion_solver(const Params& p, Domain const* d);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief			Adds the residual to the current
    ///		\brief			solution
    ///
    ///		\param mult1	now 	    *	mult1 	+
    ///		\param mult2	prev	    *	mult2	+
    ///		\param mult3	increment   *	mult3
    ///
    ////////////////////////////////////////////////////////////
    void substep(Array4d&       now,
                 const Array4d& prev,
                 const Array4d& increment,
                 double         mult1,
                 double         mult2,
                 double         mult3);

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief sets out = in (halos included)
    ///
    ////////////////////////////////////////////////////////////
    void assign_array(const Array4d& in, Array4d& out);
};

#endif
