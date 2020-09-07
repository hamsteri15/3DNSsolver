#ifndef DifferenceOperator_hh
#define DifferenceOperator_hh

#include "../containers/array_4d.hpp"

class DifferenceOperator {

public:
    DifferenceOperator() = default;

    DifferenceOperator(double dx, double dy, double dz)
        : m_dx(dx)
        , m_dy(dy)
        , m_dz(dz) {}

    virtual ~DifferenceOperator() = default;

    // first derivatives
    virtual void df_dx(const Array4d&, Array4d&) = 0;
    virtual void df_dy(const Array4d&, Array4d&) = 0;
    virtual void df_dz(const Array4d&, Array4d&) = 0;

    virtual Array4d df_dx(const Array4d&) = 0;
    virtual Array4d df_dy(const Array4d&) = 0;
    virtual Array4d df_dz(const Array4d&) = 0;

    // second derivatives
    virtual void ddf_dxdx(const Array4d&, Array4d&) = 0;
    virtual void ddf_dydy(const Array4d&, Array4d&) = 0;
    virtual void ddf_dzdz(const Array4d&, Array4d&) = 0;

    virtual Array4d ddf_dxdx(const Array4d&) = 0;
    virtual Array4d ddf_dydy(const Array4d&) = 0;
    virtual Array4d ddf_dzdz(const Array4d&) = 0;

protected:
    double m_dx, m_dy, m_dz;
};

#endif