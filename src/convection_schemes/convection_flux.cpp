#include "./convection_flux.hpp"

void ConvectionFlux::euler_flux_from_primitive(
    const double* W, double* F, double gamma, double normal_x, double normal_y, double normal_z) {

    const double rho = W[0];
    const double p   = W[1];
    const double u   = W[2];
    const double v   = W[3];
    const double w   = W[4];

    const double q_n = u * normal_x + v * normal_y + w * normal_z;          // projected velocity
    const double c = sqrt(gamma * p / rho);                                 // sound speed
    const double H = c * c / (gamma - 1.0) + 0.5 * (u * u + v * v + w * w); // enthalpy

    F[0] = rho * q_n;
    F[1] = rho * q_n * H;
    F[2] = rho * q_n * u + p * normal_x;
    F[3] = rho * q_n * v + p * normal_y;
    F[4] = rho * q_n * w + p * normal_z;
}

Array4d ConvectionFlux::form_flux(const Array4d& primitive,
                                  double         gamma,
                                  double         normal_x,
                                  double         normal_y,
                                  double         normal_z) const {

    Array4d F = Array4d(primitive.get_dimensions());

#pragma omp parallel
    {
// all cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = primitive.I_BEGIN(); i < primitive.I_END(); i++) {
            for (size_t j = primitive.J_BEGIN(); j < primitive.J_END(); j++) {
                for (size_t k = primitive.K_BEGIN(); k < primitive.K_END(); k++) {

                    double rho = primitive(i, j, k, 0);
                    double p   = primitive(i, j, k, 1);
                    double u   = primitive(i, j, k, 2);
                    double v   = primitive(i, j, k, 3);
                    double w   = primitive(i, j, k, 4);

                    double q_n = u * normal_x + v * normal_y + w * normal_z; // projected velocity
                    double c = sqrt(gamma * p / rho);                        // sound speed
                    double H = c * c / (gamma - 1.0) + 0.5 * (u * u + v * v + w * w); // enthalpy

                    F(i, j, k, 0) = rho * q_n;
                    F(i, j, k, 1) = rho * q_n * H;
                    F(i, j, k, 2) = rho * q_n * u + p * normal_x;
                    F(i, j, k, 3) = rho * q_n * v + p * normal_y;
                    F(i, j, k, 4) = rho * q_n * w + p * normal_z;
                }
            }
        }

    } // pragma

    return F;
}

std::vector<double> ConvectionFlux::orthogonal_unit_vectors(std::vector<double> unit_normal) const {

    double nx = unit_normal[0];
    double ny = unit_normal[1];
    double nz = unit_normal[2];

    double tempx = ny * ny + nz * nz;
    double tempy = nz * nz + nx * nx;
    double tempz = nx * nx + ny * ny;

    double lx, ly, lz;
    double mx, my, mz;

    if (tempx >= tempy && tempx >= tempz) {
        lx = 0.0;
        ly = -nz;
        lz = ny;
    }

    else if (tempy >= tempx && tempy >= tempz) {
        lx = -nz;
        ly = 0.0;
        lz = nx;
    }

    else if (tempz >= tempx && tempz >= tempy) {
        lx = -ny;
        ly = nx;
        lz = 0.0;
    }

    else {
        // here and exception should be thrown!
    }

    // ensure that l's are unit vectors
    double temp = sqrt(lx * lx + ly * ly + lz * lz);
    lx          = lx / temp;
    ly          = ly / temp;
    lz          = lz / temp;

    mx = ny * lz - nz * ly;
    my = nz * lx - nx * lz;
    mz = nx * ly - ny * lx;

    double abs_n_cross_l = sqrt(mx * mx + my * my + mz * mz);
    mx                   = mx / abs_n_cross_l;
    my                   = my / abs_n_cross_l;
    mz                   = mz / abs_n_cross_l;

    std::vector<double> ret = {lx, ly, lz, mx, my, mz};
    return ret;
}

void ConvectionFlux::eigenvectors(double              rho,
                                  double              p,
                                  double              u,
                                  double              v,
                                  double              w,
                                  double              gamma,
                                  std::vector<double> n,
                                  std::vector<double> l,
                                  std::vector<double> m,
                                  double*             L,
                                  double*             R) const {
    double nx = n[0];
    double ny = n[1];
    double nz = n[2];
    double lx = l[0];
    double ly = l[1];
    double lz = l[2];
    double mx = m[0];
    double my = m[1];
    double mz = m[2];

    // sound speeds
    double c2 = gamma / (p * rho);
    double c  = sqrt(c2);

    double qn = u * nx + v * ny + w * nz;
    double ql = u * lx + v * ly + w * lz;
    double qm = u * mx + v * my + w * mz;

    double q2 = u * u + v * v + w * w;
    double H  = c2 / (gamma - 1.0) + 0.5 * q2;

    double K    = gamma - 1.0;
    double K_pc = K / (2.0 * c2);

    // right eigenvectors
    double RR[5][5] = {{1.0, 0.0, 1.0, 1.0, 0.0},
                       {u - c * nx, mx, u, u + c * nx, lx},
                       {v - c * ny, my, v, v + c * ny, ly},
                       {w - c * nz, mz, w, w + c * nz, lz},
                       {H - qn * c, qm, 0.5 * q2, H + qn * c, ql}};

    double LL[5][5] = {{0.5 * K_pc * q2 + qn / (2.0 * c),
                        -(K_pc * u + nx / (2.0 * c)),
                        -(K_pc * v + ny / (2.0 * c)),
                        -(K_pc * w + nz / (2.0 * c)),
                        K_pc},
                       {-qm, mx, my, mz, 0.0},
                       {1.0 - K_pc * q2, K * u / c2, K * v / c2, K * w / c2, -K / c2},
                       {0.5 * K_pc * q2 - qn / (2.0 * c),
                        -(K_pc * u - nx / (2.0 * c)),
                        -(K_pc * v - ny / (2.0 * c)),
                        -(K_pc * w - nz / (2.0 * c)),
                        K_pc},
                       {-ql, lx, ly, lz, 0.0}};

    std::memcpy(R, RR, 5 * 5 * sizeof(double));
    std::memcpy(L, LL, 5 * 5 * sizeof(double));
}

void ConvectionFlux::eigenvalues(
    const double* W, double* eig, double gamma, double nx, double ny, double nz) {

    const double rho = W[0];
    const double p   = W[1];
    const double u   = W[2];
    const double v   = W[3];
    const double w   = W[4];

    const double q_n = u * nx + v * ny + w * nz;
    const double c   = sqrt(gamma * p / rho);

    eig[0] = q_n - c;
    eig[1] = q_n;
    eig[2] = q_n + c;
    eig[3] = q_n;
    eig[4] = q_n;
}

double
ConvectionFlux::max_eigenvalue(const double* W, double gamma, double nx, double ny, double nz) {

    const double rho = W[0];
    const double p   = W[1];
    const double u   = W[2];
    const double v   = W[3];
    const double w   = W[4];

    const double q_n = u * nx + v * ny + w * nz;
    const double c   = sqrt(gamma * p / rho);

    double eig1 = std::abs(q_n - c);
    double eig2 = std::abs(q_n + c);
    return std::max(eig1, eig2);
}

Array3d ConvectionFlux::max_eigenvalue(const Array4d& primitive,
                                       double         gamma,
                                       double         normal_x,
                                       double         normal_y,
                                       double         normal_z) const {

    Array3d eig = Array3d(primitive.get_dimensions());

#pragma omp parallel
    {
// all cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = primitive.I_BEGIN(); i < primitive.I_END(); i++) {
            for (size_t j = primitive.J_BEGIN(); j < primitive.J_END(); j++) {
                for (size_t k = primitive.K_BEGIN(); k < primitive.K_END(); k++) {

                    double rho = primitive(i, j, k, 0);
                    double p   = primitive(i, j, k, 1);
                    double u   = primitive(i, j, k, 2);
                    double v   = primitive(i, j, k, 3);
                    double w   = primitive(i, j, k, 4);

                    const double q_n = u * normal_x + v * normal_y + w * normal_z;
                    const double c   = sqrt(gamma * p / rho);

                    double eig1 = std::abs(q_n - c);
                    double eig2 = std::abs(q_n + c);

                    eig(i, j, k) = 3.0; // std::max(eig1, eig2);
                }
            }
        }

    } // pragma

    return eig;
}
