#ifndef CenteredConvection_hh
#define CenteredConvection_hh

#include "convection_scheme.hpp"
#include "difference_operators/cd-n.hpp"

class CenteredConvection : public ConvectionScheme {

public:
    CenteredConvection()
        : ConvectionScheme() {}

    CenteredConvection(std::string type, double gamma, size_t order)
        : ConvectionScheme(type, gamma)
        , m_order(order) {}

    ~CenteredConvection() {}

    virtual void solve(const Grid&    grid,
                       const Array4d& primitive,
                       const Array4d& conservative,
                       Array4d&       residual) {}

protected:
private:
    size_t m_order;
    void   compute_residual(Array4d& residual);
};

#endif
