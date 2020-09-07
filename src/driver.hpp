#ifndef driver_hh
#define driver_hh

#include <mpi.h>
#include <omp.h>

#include "domain.hpp"
#include "params.hpp"
#include "timer.hpp"

#include "time_scheme.hpp"

#include "convection_schemes/centered_convection.hpp"
#include "convection_schemes/weno-5.hpp"
#include "convection_schemes/weno-5cw.hpp"

#include "diffusion_schemes/centered_diffusion.hpp"

#include "io/hdf5_writer.hpp"
#include "io/io_binary.hpp"
#include "io/output_info.hpp"
#include "io/reader.hpp"
#include "testcases.hpp"

class Driver {

public:
    Driver() {}
    ~Driver() {}

    Params read_parameters();
    void   set_writer();

    ////////////////////////////////////////////////////////////
    ///
    ///    \Runs the solver
    ///
    ////////////////////////////////////////////////////////////
    void run();

    ////////////////////////////////////////////////////////////
    ///
    ///    \Sets the initial condition based on parameters file
    ///     should be modified for restarting
    ///
    ///////////////////////////////////////////////////////////
    void set_initial_condition(Domain&             domain,
                               const Grid&         grid,
                               const Params&       p,
                               const Communicator& comm);

    ////////////////////////////////////////////////////////////
    ///
    ///    \Get current time
    ///
    ////////////////////////////////////////////////////////////
    double get_time() const;

private:
    ConvectionScheme* create_convection_operator(const Params& p) const;
    DiffusionScheme*  create_diffusion_operator(const Params& p) const;
};

#endif
