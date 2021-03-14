#include "boundary.hpp"

Boundary::Boundary(std::string type, int location, const Array4dDimensions& ad)
    : m_type(type)
    , m_location(location)
    , m_ngc(ad.ngc)
    , m_nx(ad.nx)
    , m_ny(ad.ny)
    , m_nz(ad.nz) {

    m_gc_start_west  = 0;
    m_gc_start_east  = m_nx + m_ngc;
    m_gc_start_north = 0;
    m_gc_start_south = m_ny + m_ngc;
    m_gc_start_back  = 0;
    m_gc_start_front = m_nz + m_ngc;

    m_q_start = 0;
    m_q_end   = ad.nq;
}

Boundary::Boundary(std::string type, int location, const Array4d& arr)
    : m_type(type)
    , m_location(location) {

    m_ngc = arr.ngc();
    m_nx  = arr.nx();
    m_ny  = arr.ny();
    m_nz  = arr.nz();

    m_gc_start_west  = 0;
    m_gc_start_east  = m_nx + m_ngc;
    m_gc_start_north = 0;
    m_gc_start_south = m_ny + m_ngc;
    m_gc_start_back  = 0;
    m_gc_start_front = m_nz + m_ngc;

    m_q_start = 0;
    m_q_end   = arr.nq();
}
