#include "centered_diffusion.hpp"

void CenteredDiffusion::solve(const Grid&         grid,
                              const Communicator& communicator,
                              const Array4d&      primitive,
                              Array4d&            residual) {

    // Array4d W_temp = divide_p_by_rho(primitive);
    // communicator.communicate(W_temp);

    CDN difference_operator = CDN(m_order, grid.dx(), grid.dy(), grid.dz());

    Array4d ddW_dxdx = difference_operator.ddf_dxdx(primitive);
    Array4d ddW_dydy = difference_operator.ddf_dydy(primitive);
    Array4d ddW_dzdz = difference_operator.ddf_dzdz(primitive);

    Array4d dW_dx = difference_operator.df_dx(primitive);
    Array4d dW_dy = difference_operator.df_dy(primitive);
    Array4d dW_dz = difference_operator.df_dz(primitive);

    // these have to be communicated before cross derivatives can be computed
    communicator.communicate(dW_dx);
    communicator.communicate(dW_dy);
    communicator.communicate(dW_dz);

    // MPI_Comm temp = communicator.get_communicator();
    /*
    int world_rank;
    MPI_Comm_rank(communicator.get_communicator(), &world_rank);



    if (world_rank == 0){

        Writer writer = Writer("asd");

        writer.print_4d_array_slice(dW_dx, 1, 3); //
        //writer.print_4d_array_slice(dW_dz, 1, 3);
        std::exit(1);


    }
    */

    Array4d dW_dxdy = difference_operator.df_dy(dW_dx);
    Array4d dW_dxdz = difference_operator.df_dz(dW_dx);

    Array4d dW_dydx = difference_operator.df_dx(dW_dy);
    Array4d dW_dydz = difference_operator.df_dz(dW_dy);

    Array4d dW_dzdx = difference_operator.df_dx(dW_dz);
    Array4d dW_dzdy = difference_operator.df_dy(dW_dz);

    compute_residual(primitive,
                     dW_dx,
                     dW_dy,
                     dW_dz,
                     ddW_dxdx,
                     ddW_dydy,
                     ddW_dzdz,
                     dW_dxdy,
                     dW_dxdz,
                     dW_dydx,
                     dW_dydz,
                     dW_dzdx,
                     dW_dzdy,
                     residual);
}

void CenteredDiffusion::compute_residual(const Array4d& primitive,
                                         const Array4d& Dx,
                                         const Array4d& Dy,
                                         const Array4d& Dz,
                                         const Array4d& Dxx,
                                         const Array4d& Dyy,
                                         const Array4d& Dzz,
                                         const Array4d& Dxy,
                                         const Array4d& Dxz,
                                         const Array4d& Dyx,
                                         const Array4d& Dyz,
                                         const Array4d& Dzx,
                                         const Array4d& Dzy,
                                         Array4d&       residual) {

#pragma omp parallel
    {

        enum indices { rho, p, u, v, w } idx;

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = residual.i_begin(); i < residual.i_end(); i++) {
            for (size_t j = residual.j_begin(); j < residual.j_end(); j++) {
                for (size_t k = residual.k_begin(); k < residual.k_end(); k++) {

                    double mu     = m_mu;
                    double dmu_dx = 0.0;
                    double dmu_dy = 0.0;
                    double dmu_dz = 0.0;

                    double lamb     = (-2. / 3.) * mu;
                    double dlamb_dx = -(2. / 3.) * dmu_dx;
                    double dlamb_dy = -(2. / 3.) * dmu_dy;

                    double U   = primitive(i, j, k, u);
                    double V   = primitive(i, j, k, v);
                    double P   = primitive(i, j, k, p);
                    double RHO = primitive(i, j, k, rho);

                    // u-derivatives
                    double du_dx   = Dx(i, j, k, u);
                    double du_dy   = Dy(i, j, k, u);
                    double du_dxdx = Dxx(i, j, k, u);
                    double du_dydy = Dyy(i, j, k, u);
                    double du_dxdy = Dxy(i, j, k, u);
                    double du_dydx = Dyx(i, j, k, u);

                    // v-derivatives
                    double dv_dy   = Dy(i, j, k, v);
                    double dv_dx   = Dx(i, j, k, v);
                    double dv_dxdx = Dxx(i, j, k, v);
                    double dv_dydy = Dyy(i, j, k, v);
                    double dv_dydx = Dyx(i, j, k, v);
                    double dv_dxdy = Dxy(i, j, k, v);

                    double drho_dx   = Dx(i, j, k, rho);
                    double dp_dx     = Dx(i, j, k, p);
                    double drho_dxdx = Dxx(i, j, k, rho);
                    double dp_dxdx   = Dxx(i, j, k, p);

                    double drho_dy   = Dy(i, j, k, rho);
                    double dp_dy     = Dy(i, j, k, p);
                    double drho_dydy = Dyy(i, j, k, rho);
                    double dp_dydy   = Dyy(i, j, k, p);

                    double Dtauxx_dx =
                        dtauxx_dx(mu, dmu_dx, du_dx, dv_dy, du_dxdx, dv_dydx, lamb, dlamb_dx);
                    double Dtauyy_dy =
                        dtauyy_dy(mu, dmu_dy, dv_dy, dv_dydy, du_dx, du_dxdy, lamb, dlamb_dy);
                    double Dtauxy_dx = dtauxy_dx(mu, dmu_dx, du_dy, dv_dx, du_dydx, dv_dxdx);
                    double Dtauyx_dy = dtauyx_dy(mu, dmu_dy, du_dy, dv_dx, du_dydy, dv_dxdy);
                    double Tauxx     = tauxx(mu, du_dx, dv_dy, lamb);
                    double Tauyy     = tauxx(mu, du_dx, dv_dy, lamb);
                    double Tauxy     = tauxy(mu, du_dy, dv_dx);
                    double Tauyx     = Tauxy;

                    double Dtauvx_dx =
                        U * Dtauxx_dx + V * Dtauxy_dx + Tauxx * du_dx + Tauxy * dv_dy;
                    double Dtauvy_dy =
                        U * Dtauyx_dy + V * Dtauyy_dy + Tauyy * dv_dy + Tauyx * du_dy;

                    double one_rho  = 1. / RHO;
                    double one_rho2 = 1. / (RHO * RHO);
                    double one_rho3 = 1. / (RHO * RHO * RHO);

                    // derivative of heat flux x-dir
                    double temp1 = dmu_dx * (one_rho * dp_dx - P * one_rho2 * drho_dx);
                    double temp2 =
                        mu * (2.0 * one_rho2 * drho_dx * dp_dx + one_rho * dp_dxdx +
                              one_rho3 * P * drho_dx * drho_dx - one_rho2 * P * drho_dxdx);
                    double temp3  = m_gamma / (m_Pr * (m_gamma - 1.0));
                    double Dqx_dx = -temp3 * (temp1 + temp2);

                    temp1         = dmu_dy * (one_rho * dp_dy - P * one_rho2 * drho_dy);
                    temp2         = mu * (2.0 * one_rho2 * drho_dy * dp_dy + one_rho * dp_dydy +
                                  one_rho3 * p * drho_dy * drho_dy - one_rho2 * P * drho_dydy);
                    double Dqy_dy = -temp3 * (temp1 + temp2);

                    double x_mom = -Dtauxx_dx - Dtauyx_dy;
                    double y_mom = -Dtauyy_dy - Dtauxy_dx;
                    double ene   = -Dtauvx_dx - Dtauvy_dy + Dqx_dx + Dqy_dy;

                    residual(i, j, k, p) += ene;
                    residual(i, j, k, u) += x_mom;
                    residual(i, j, k, v) += y_mom;
                    residual(i, j, k, w) -= 0.0;
                }
            }
        }

    } // pragma
}

void CenteredDiffusion::initialize_temp_arrays(Array4d& primitive) {

    /*
    #pragma omp parallel
    {

    ///		W[i][j][k][0]		rho
    ///		W[i][j][k][1]		p
    ///		W[i][j][k][2]		u
    ///		W[i][j][k][3]		v
    ///		W[i][j][k][4]		w

    ///		m_W_temp[i][j][k][0]		0.0
    ///		m_W_temp[i][j][k][1]		p/rho
    ///		m_W_temp[i][j][k][2]		u
    ///		m_W_temp[i][j][k][3]		v
    ///		m_W_temp[i][j][k][4]		w


    Array_4d W = primitive.get_data();
    Array_4d Wtemp = m_W_temp.get_data();

    //all cells loop
    #pragma omp for	schedule(dynamic)
    for (size_t i = primitive.I_BEGIN(); i < primitive.I_END(); i++){
    for (size_t j = primitive.J_BEGIN(); j < primitive.J_END(); j++){
    for (size_t k = primitive.K_BEGIN(); k < primitive.K_END(); k++){

        Wtemp[i][j][k][0] = 0.0;
        Wtemp[i][j][k][1] = W[i][j][k][1] / W[i][j][k][0]; //p/rho
        Wtemp[i][j][k][2] = W[i][j][k][2];
        Wtemp[i][j][k][3] = W[i][j][k][3];
        Wtemp[i][j][k][4] = W[i][j][k][4];

    }}} //i j k

    }//pragma
    */
}
