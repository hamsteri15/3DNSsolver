#ifndef ConvectionScheme_hh
#define ConvectionScheme_hh

#include <iostream>

#include "containers/array_4d.hpp"
#include "convection_schemes/convection_constants.hpp"
#include "convection_schemes/convection_flux.hpp"
#include "grid.hpp"
#include "io/writer.hpp"
#include "params.hpp"
#include "timer.hpp"

class ConvectionScheme {

public:
    // default
    ConvectionScheme() = default;

    ConvectionScheme(std::string type, double gamma)
        : m_type(type)
        , m_gamma(gamma) {}

    virtual ~ConvectionScheme() = default;

    virtual void solve(const Grid&, const Array4d&, const Array4d&, Array4d&) {}

    inline std::string get_type() { return m_type; };

protected:
    std::string m_type;
    double      m_gamma;
};

#endif
