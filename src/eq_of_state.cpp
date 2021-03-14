#include "eq_of_state.hpp"

double EqOfState::E(double rho, double p, double u, double v, double w) {

    return p / ((m_gamma - 1.0) * rho) + 0.5 * (u * u + v * v + w * w);
}

/*
void EqOfState::primitive_to_conservative(const double* W, double* U){


    U[0] = W[0]; 		//rho
    U[2] = W[0]*W[2]; 	//rho*u
    U[3] = W[0]*W[3]; 	//rho*v
    U[4] = W[0]*W[4];	//rho*w
    U[1] = W[1]/(m_gamma-1.0) + 0.5 * W[0] * ( W[2]*W[2] + W[3]*W[3] + W[4]*W[4] ) ; //rho*E


}

void EqOfState::conservative_to_primitive(const double* U, double* W){

    W[0] = U[0]; 		//rho
    W[2] = U[2]/U[0];  	//u
    W[3] = U[3]/U[0];  	//v
    W[4] = U[4]/U[0];  	//w
    W[1] = (m_gamma - 1.0) * ( U[1] - 0.5 * U[0] * ( W[2]*W[2] + W[3]*W[3] + W[4]*W[4] ) ); //p

}
*/

void EqOfState::primitive_to_conservative(const Array4d& W, Array4d& U) {

#pragma omp parallel
    {

// loops here have to include the barrier nodes!
#pragma omp for schedule(dynamic)
        for (size_t i = W.I_BEGIN(); i < W.I_END(); i++) {
            for (size_t j = W.J_BEGIN(); j < W.J_END(); j++) {
                for (size_t k = W.K_BEGIN(); k < W.K_END(); k++) {

                    U(i, j, k, 0) = W(i, j, k, 0);                 // rho
                    U(i, j, k, 2) = W(i, j, k, 0) * W(i, j, k, 2); // rho*u
                    U(i, j, k, 3) = W(i, j, k, 0) * W(i, j, k, 3); // rho*v
                    U(i, j, k, 4) = W(i, j, k, 0) * W(i, j, k, 4); // rho*w

                    // 0.5 * (u*u + v*v + w*w)
                    double kin = 0.5 * W(i, j, k, 0) *
                                 (W(i, j, k, 2) * W(i, j, k, 2) + W(i, j, k, 3) * W(i, j, k, 3) +
                                  W(i, j, k, 4) * W(i, j, k, 4));

                    U(i, j, k, 1) = W(i, j, k, 1) / (m_gamma - 1.0) + kin; // rho*E
                }
            }
        } // ijk

    } // pragma
}

void EqOfState::conservative_to_primitive(const Array4d& U, Array4d& W) {

#pragma omp parallel
    {

// loops here have to include the barrier nodes!
#pragma omp for schedule(dynamic)
        for (size_t i = W.I_BEGIN(); i < W.I_END(); i++) {
            for (size_t j = W.J_BEGIN(); j < W.J_END(); j++) {
                for (size_t k = W.K_BEGIN(); k < W.K_END(); k++) {

                    W(i, j, k, 0) = U(i, j, k, 0);                 // rho
                    W(i, j, k, 2) = U(i, j, k, 2) / U(i, j, k, 0); // u
                    W(i, j, k, 3) = U(i, j, k, 3) / U(i, j, k, 0); // v
                    W(i, j, k, 4) = U(i, j, k, 4) / U(i, j, k, 0); // w

                    // 0.5 * (u*u + v*v + w*w)
                    double kin =
                        0.5 * (W(i, j, k, 2) * W(i, j, k, 2) + W(i, j, k, 3) * W(i, j, k, 3) +
                               W(i, j, k, 4) * W(i, j, k, 4));

                    W(i, j, k, 1) = (m_gamma - 1.0) * (U(i, j, k, 1) - U(i, j, k, 0) * kin);
                }
            }
        } // ijk
    }     // pragma
}

double EqOfState::local_dt(
    double rho, double p, double u, double v, double w, double dx, double dy, double dz) {

    double c = sqrt(m_gamma * p / rho);

    double eig_x = dx / std::max(std::abs(u + c), std::abs(u - c));
    double eig_y = dy / std::max(std::abs(v + c), std::abs(v - c));
    double eig_z = dz / std::max(std::abs(w + c), std::abs(w - c));

    double tmp = std::min(eig_x, eig_y);

    return std::min(tmp, eig_z);
}
