#include "cd-n.hpp"

void CDN::initialize_coefficients(int order) {

    m_df_coeffs  = new double[order + 1];
    m_ddf_coeffs = new double[order + 1];

    if (order == 2) {
        m_stencil_w = cd2_stencil_width;
        m_start_idx = cd2_start_idx;
        std::memcpy(m_df_coeffs, cd2_df_coeff, cd2_stencil_width * sizeof(double));
        std::memcpy(m_ddf_coeffs, cd2_ddf_coeff, cd2_stencil_width * sizeof(double));

    }

    else if (order == 4) {
        m_stencil_w = cd4_stencil_width;
        m_start_idx = cd4_start_idx;
        std::memcpy(m_df_coeffs, cd4_df_coeff, cd4_stencil_width * sizeof(double));
        std::memcpy(m_ddf_coeffs, cd4_ddf_coeff, cd4_stencil_width * sizeof(double));

    }

    else if (order == 6) {
        m_stencil_w = cd6_stencil_width;
        m_start_idx = cd6_start_idx;
        std::memcpy(m_df_coeffs, cd6_df_coeff, cd6_stencil_width * sizeof(double));
        std::memcpy(m_ddf_coeffs, cd6_ddf_coeff, cd6_stencil_width * sizeof(double));
    }
}

void CDN::df_dx(const Array4d& f, Array4d& df) { generic_fd_x(f, df, m_df_coeffs, m_dx); }
void CDN::df_dy(const Array4d& f, Array4d& df) { generic_fd_y(f, df, m_df_coeffs, m_dy); }
void CDN::df_dz(const Array4d& f, Array4d& df) { generic_fd_z(f, df, m_df_coeffs, m_dy); }

void CDN::ddf_dxdx(const Array4d& f, Array4d& df) {
    generic_fd_x(f, df, m_ddf_coeffs, m_dx * m_dx);
}
void CDN::ddf_dydy(const Array4d& f, Array4d& df) {
    generic_fd_y(f, df, m_ddf_coeffs, m_dy * m_dy);
}
void CDN::ddf_dzdz(const Array4d& f, Array4d& df) {
    generic_fd_z(f, df, m_ddf_coeffs, m_dz * m_dz);
}

Array4d CDN::df_dx(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_x(f, df, m_df_coeffs, m_dx);
    return df;
}

Array4d CDN::df_dy(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_y(f, df, m_df_coeffs, m_dy);
    return df;
}

Array4d CDN::df_dz(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_z(f, df, m_df_coeffs, m_dz);
    return df;
}

Array4d CDN::ddf_dxdx(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_x(f, df, m_ddf_coeffs, m_dx * m_dx);
    return df;
}

Array4d CDN::ddf_dydy(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_x(f, df, m_ddf_coeffs, m_dy * m_dy);
    return df;
}

Array4d CDN::ddf_dzdz(const Array4d& f) {

    Array4d df = Array4d(f.get_dimensions());
    generic_fd_x(f, df, m_ddf_coeffs, m_dz * m_dz);
    return df;
}

////////////////////////
// This is where all the heavy lifting happens
////////////////////////
void CDN::generic_fd_x(const Array4d& f, Array4d& df, double* coeffs, const double d) {

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = f.i_begin(); i < f.i_end(); i++) {
            for (size_t j = f.j_begin(); j < f.j_end(); j++) {
                for (size_t k = f.k_begin(); k < f.k_end(); k++) {
                    for (size_t q = f.q_begin(); q < f.q_end(); q++) {

                        double sum = 0.;
                        for (size_t idx = 0; idx < m_stencil_w; idx++) {

                            sum += f(i + m_start_idx + idx, j, k, q) * coeffs[idx] / d;
                        }
                        df(i, j, k, q) = sum;
                    }
                }
            }
        } // i j k q

    } // pragma
}

void CDN::generic_fd_y(const Array4d& f, Array4d& df, double* coeffs, const double d) {

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = f.i_begin(); i < f.i_end(); i++) {
            for (size_t j = f.j_begin(); j < f.j_end(); j++) {
                for (size_t k = f.k_begin(); k < f.k_end(); k++) {
                    for (size_t q = f.q_begin(); q < f.q_end(); q++) {

                        double sum = 0.;
                        for (size_t idx = 0; idx < m_stencil_w; idx++) {
                            sum += f(i, j + m_start_idx + idx, k, q) * coeffs[idx] / d;
                        }
                        df(i, j, k, q) = sum;
                    }
                }
            }
        } // i j k q

    } // pragma
}

void CDN::generic_fd_z(const Array4d& f, Array4d& df, double* coeffs, const double d) {

#pragma omp parallel
    {

// inner cells loop
#pragma omp for schedule(dynamic)
        for (size_t i = f.i_begin(); i < f.i_end(); i++) {
            for (size_t j = f.j_begin(); j < f.j_end(); j++) {
                for (size_t k = f.k_begin(); k < f.k_end(); k++) {
                    for (size_t q = f.q_begin(); q < f.q_end(); q++) {

                        double sum = 0.;
                        for (size_t idx = 0; idx < m_stencil_w; idx++) {
                            sum += f(i, j, k + m_start_idx + idx, q) * coeffs[idx] / d;
                        }
                        df(i, j, k, q) = sum;
                    }
                }
            }
        } // i j k q

    } // pragma
}