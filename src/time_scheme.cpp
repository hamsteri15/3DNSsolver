#include "time_scheme.hpp"

void TimeScheme::step(Domain&             domain,
                      const Grid&         grid,
                      const Communicator& communicator,
                      ConvectionScheme*   convection,
                      DiffusionScheme*    diffusion) {

    Array4d& U = domain.get_NS_conservative_array();
    Array4d& W = domain.get_NS_primitive_array();

    Array4d R     = Array4d(W.get_dimensions()); // residual
    Array4d Uprev = Array4d(W.get_dimensions()); // previous

    // adjust dt
    set_dt(W, grid);

    /*
    std::cout << "..." << std::endl;
    writer.print_4d_array_slice(U, 2, 3);
    std::exit(1);
    */

    // maybe unnecessary
    m_eq_of_state.primitive_to_conservative(W, U);

    assign_array(U, Uprev); // set Uprev = U

    // RK-3 routine
    double mult1, mult2, mult3;

    // 1st Step
    R.set_zero();
    domain.apply_boundary_conditions(U);
    communicator.communicate(U);
    m_eq_of_state.conservative_to_primitive(U, W); // to update W halos
    mult1 = 1.0;
    mult2 = 0.0;
    mult3 = -m_current_dt;
    convection->solve(grid, W, U, R);
    diffusion->solve(grid, communicator, W, R);
    substep(U, Uprev, R, mult1, mult2, mult3);

    // 2nd Step
    R.set_zero();
    domain.apply_boundary_conditions(U);
    communicator.communicate(U);
    m_eq_of_state.conservative_to_primitive(U, W);
    mult1 = 3.0 / 4.0;
    mult2 = 1.0 / 4.0;
    mult3 = -(1. / 4.) * m_current_dt;
    convection->solve(grid, W, U, R);
    diffusion->solve(grid, communicator, W, R);
    substep(U, Uprev, R, mult1, mult2, mult3);

    // 3rd Step
    R.set_zero();
    domain.apply_boundary_conditions(U);
    communicator.communicate(U);
    m_eq_of_state.conservative_to_primitive(U, W); // to update W halos
    mult1 = 1. / 3.;
    mult2 = 2. / 3.;
    mult3 = -(2. / 3.) * m_current_dt;
    convection->solve(grid, W, U, R);
    diffusion->solve(grid, communicator, W, R);
    substep(U, Uprev, R, mult1, mult2, mult3);

    // boundary conditions are not applied in W now
    m_eq_of_state.conservative_to_primitive(U, W);
}

void TimeScheme::substep(Array4d&       now,
                         const Array4d& prev,
                         const Array4d& increment,
                         double         mult1,
                         double         mult2,
                         double         mult3) {

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = now.i_begin(); i < now.i_end(); i++) {
            for (size_t j = now.j_begin(); j < now.j_end(); j++) {
                for (size_t k = now.k_begin(); k < now.k_end(); k++) {
                    for (size_t q = now.q_begin(); q < now.q_end(); q++) {

                        now(i, j, k, q) = (prev(i, j, k, q) * mult1) + (now(i, j, k, q) * mult2) +
                                          (increment(i, j, k, q) * mult3);
                    }
                }
            }
        } // i j k q
    }     // pragma
}

double TimeScheme::get_local_max_dt(const Array4d& primitive, const Grid& grid) {

    double dx = grid.dx();
    double dy = grid.dy();
    double dz = grid.dz();

    double max_dt = 1E10;

#pragma omp parallel for reduction(min : max_dt)
    for (size_t i = primitive.i_begin(); i < primitive.i_end(); i++) {
        for (size_t j = primitive.j_begin(); j < primitive.j_end(); j++) {
            for (size_t k = primitive.k_begin(); k < primitive.k_end(); k++) {

                double rho = primitive(i, j, k, 0);
                double p   = primitive(i, j, k, 1);
                double u   = primitive(i, j, k, 2);
                double v   = primitive(i, j, k, 3);
                double w   = primitive(i, j, k, 4);

                double dt = m_eq_of_state.local_dt(rho, p, u, v, w, dx, dy, dz);

                if (dt < max_dt) { max_dt = dt; }
            }
        }
    }

    return max_dt;
}

void TimeScheme::set_dt(const Array4d& primitive, const Grid& grid) {

    double local_dt = get_local_max_dt(primitive, grid) * m_CFL_max;

    double global_dt;

    MPI_Allreduce(&local_dt, &global_dt, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);

    m_current_dt = global_dt;
}

void TimeScheme::assign_array(const Array4d& in, Array4d& out) {

#pragma omp parallel
    {

#pragma omp for schedule(dynamic)
        for (size_t i = in.i_begin(); i < in.i_end(); i++) {
            for (size_t j = in.j_begin(); j < in.j_end(); j++) {
                for (size_t k = in.k_begin(); k < in.k_end(); k++) {
                    for (size_t q = in.q_begin(); q < in.q_end(); q++) {

                        out(i, j, k, q) = in(i, j, k, q);
                    }
                }
            }
        } // i j k q

    } // pramga
}

/*


void Solver::create_convection_solver(const Params& p, Domain const* d){



    if (p.convection_scheme == "Weno") {

        if (p.convection_in_characteristic_space){
            m_convection_scheme = new Weno5cw(p,d);


        }
        else{
            m_convection_scheme = new Weno5(p,d);

        }


    }

    else if (p.convection_scheme == "Centered") { m_convection_scheme = new CenteredConvection(p,d);
}

    else{

        m_convection_scheme = new ConvectionScheme();
    }





}

void Solver::create_diffusion_solver(const Params& p, Domain const* d){


    if (p.diffusion_scheme == "Centered") { m_diffusion_scheme = new CenteredDiffusion(p,d);}

    else if (p.diffusion_scheme == "Shen") { m_diffusion_scheme = new Shen(p,d);}

    else {
        m_diffusion_scheme = new DiffusionScheme();
    }




}

*/
