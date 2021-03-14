#include "driver.hpp"

void Driver::run() {

    // read params
    Params parameters = read_parameters();

    // decompose domain
    Decomposition decomposition = Decomposition(parameters);

    // create grid
    Grid grid = Grid(parameters, decomposition);

    // create communicator
    Communicator communicator = Communicator(decomposition);

    // create domain
    Domain domain = Domain(parameters, decomposition);

    // set initial condition
    set_initial_condition(domain, grid, parameters, communicator);

    ConvectionScheme* convection_operator = create_convection_operator(parameters);
    DiffusionScheme*  diffusion_operator  = create_diffusion_operator(parameters);
    TimeScheme        time_integrator     = TimeScheme(parameters);

    double end_time = parameters.end_t;
    double CFL_max  = parameters.CFL_max;

    // PLACE ME TO WRITER CLASS
    int io_interval = parameters.io_interval;

    OutputInfo output_info =
        OutputInfo(parameters.io_interval, "3d fields", "results/primitive_fields_3d.h5");

    Hdf5Writer writer = Hdf5Writer();

    double time = 0.0;
    int    iter = 0;

    Timer timer;

    while (time < end_time) {
        // for (int i = 0; i < 3; i++){
        timer.start();

        output_info.update(time, iter);

        // writer.write(output_info, grid, domain, decomposition, communicator, parameters );

        time_integrator.step(domain, grid, communicator, convection_operator, diffusion_operator);

        double dt = time_integrator.get_dt();

        iter++;

        time = time + dt;

        timer.stop();

        if (domain.get_rank() == 0) {

            std::cout << "Flow time: " << time << "\tCPU time (step): " << timer.get_elapsed()
                      << "\tdt: " << dt << "\tCFL: " << CFL_max << std::endl;
        }

        timer.reset();
    }

    delete convection_operator;
    delete diffusion_operator;
}

ConvectionScheme* Driver::create_convection_operator(const Params& p) const {

    if (p.convection_scheme == "Weno") {

        if (p.convection_in_characteristic_space) {
            return new Weno5cw("Weno-5cw", p.gamma, p.weight_type);

        } else {
            return new Weno5("Weno-5cw", p.gamma, p.weight_type);
        }

    }

    else if (p.convection_scheme == "Centered") {
        return new CenteredConvection("Centered", p.gamma, p.convection_scheme_order);
    }

    else {
        return new ConvectionScheme("null", p.gamma);
    }
}

DiffusionScheme* Driver::create_diffusion_operator(const Params& p) const {

    if (p.diffusion_scheme == "Centered") {
        return new CenteredDiffusion("Centered", p.gamma, p.mu, p.Pr, p.diffusion_scheme_order);
    }

    else {
        return new DiffusionScheme();
    }
}

void Driver::set_initial_condition(Domain&             domain,
                                   const Grid&         grid,
                                   const Params&       p,
                                   const Communicator& comm) {

    if (p.testcase == "sod_x") {
        testcases::sod_xdir(domain, grid, p);
    }

    else if (p.testcase == "sod_y") {
        testcases::sod_ydir(domain, grid, p);
    } else if (p.testcase == "sod_z") {
        testcases::sod_zdir(domain, grid, p);
    } else if (p.testcase == "case3") {
        testcases::case3(domain, grid, p);
    } else if (p.testcase == "case4") {
        testcases::case4(domain, grid, p);
    } else if (p.testcase == "case6") {
        testcases::case6(domain, grid, p);
    } else if (p.testcase == "case12") {
        testcases::case12(domain, grid, p);
    } else if (p.testcase == "case15") {
        testcases::case15(domain, grid, p);
    } else if (p.testcase == "case17") {
        testcases::case17(domain, grid, p);
    } else if (p.testcase == "shear_xy") {
        testcases::shear_layer_xy(domain, grid, p);
    } else if (p.testcase == "shear_3d") {
        testcases::shear_layer_3d(domain, grid, p);
    }

    // W is initialized below, this ensures that barrier nodes are intialized
    // in both U and W. Dirty, I know...
    /*EqOfState eos = EqOfState(p.gamma);
    Array4d& W = domain.get_NS_primitive_array();
    Array4d& U = domain.get_NS_conservative_array();
    eos.primitive_to_conservative(W,U);
    domain.apply_boundary_conditions(U);
    comm.communicate(U);
    eos.conservative_to_primitive(U,W);
    */
}

void Driver::set_writer() {}

Params Driver::read_parameters() {

    Params p      = Params();
    Reader reader = Reader("input.xml");
    reader.read_input_file(p);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_rank == 0) { p.sanity_check(); }

    return p;
}
