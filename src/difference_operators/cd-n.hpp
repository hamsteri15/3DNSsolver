#ifndef CDN_hh
#define CDN_hh

#include "difference_operators/cd_constants.hpp"
#include "difference_operators/difference_operator.hpp"
#include <cstring> //memcpy
#include <iostream>

class CDN : public DifferenceOperator {

public:
    CDN() = default;

    CDN(size_t order, double dx, double dy, double dz)
        : DifferenceOperator(dx, dy, dz) {

        initialize_coefficients(order);
    }

    ~CDN() {
        delete[] m_df_coeffs;
        delete[] m_ddf_coeffs;
    }

    // first derivatives
    virtual void df_dx(const Array4d& f, Array4d& df);
    virtual void df_dy(const Array4d& f, Array4d& df);
    virtual void df_dz(const Array4d& f, Array4d& df);

    virtual Array4d df_dx(const Array4d& f);
    virtual Array4d df_dy(const Array4d& f);
    virtual Array4d df_dz(const Array4d& f);

    // second derivatives
    virtual void ddf_dxdx(const Array4d& f, Array4d& df);
    virtual void ddf_dydy(const Array4d& f, Array4d& df);
    virtual void ddf_dzdz(const Array4d& f, Array4d& df);

    virtual Array4d ddf_dxdx(const Array4d& f);
    virtual Array4d ddf_dydy(const Array4d& f);
    virtual Array4d ddf_dzdz(const Array4d& f);

protected:
    size_t m_stencil_w;
    size_t m_start_idx;

    double* m_df_coeffs;
    double* m_ddf_coeffs;

private:
    void generic_fd_x(const Array4d& f, Array4d& df, double* coeffs, const double d);
    void generic_fd_y(const Array4d& f, Array4d& df, double* coeffs, const double d);
    void generic_fd_z(const Array4d& f, Array4d& df, double* coeffs, const double d);
    void initialize_coefficients(int order);
};

#endif
