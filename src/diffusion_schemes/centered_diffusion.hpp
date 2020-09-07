#ifndef CenteredDiffusion_hh
#define CenteredDiffusion_hh

#include "difference_operators/cd-n.hpp"
#include "difference_operators/difference_operator.hpp"
#include "diffusion_schemes/diffusion_scheme.hpp"

class CenteredDiffusion : public DiffusionScheme {

public:
    CenteredDiffusion()
        : DiffusionScheme() {}

    CenteredDiffusion(std::string type, double gamma, double mu, double Pr, size_t order)
        : DiffusionScheme(type, gamma, mu, Pr)
        , m_order(order) {}

    virtual void solve(const Grid&         grid,
                       const Communicator& communicator,
                       const Array4d&      primitive,
                       Array4d&            residual);

    ~CenteredDiffusion() {}

protected:
private:
    size_t m_order;

    void initialize_temp_arrays(Array4d& primitive);

    void compute_residual(const Array4d& primitive,
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
                          Array4d&       residual);
};

#endif
