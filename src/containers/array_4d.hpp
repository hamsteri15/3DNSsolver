#ifndef Array4d_hh
#define Array4d_hh

#include "array_4d_dimensions.hpp"
#include <iostream>
#include <mpi.h>
#include <stdlib.h>

class Array4d {

public:
    Array4d();
    Array4d(const Array4dDimensions& dimensions);
    Array4d(
        const size_t nx, const size_t ny, const size_t nz, const size_t nq, const size_t nbarriers);

    ~Array4d();

    const Array4dDimensions& get_dimensions() const { return m_dimensions; }

    size_t nx() const { return m_dimensions.nx; }
    size_t ny() const { return m_dimensions.ny; }
    size_t nz() const { return m_dimensions.nz; }
    size_t nq() const { return m_dimensions.nq; }
    size_t ngc() const { return m_dimensions.ngc; }

    size_t NX() const { return m_dimensions.NX; }
    size_t NY() const { return m_dimensions.NY; }
    size_t NZ() const { return m_dimensions.NZ; }
    size_t NQ() const { return m_dimensions.nq; }

    // inner points loops
    size_t i_begin() const { return m_dimensions.ngc; }
    size_t j_begin() const { return m_dimensions.ngc; }
    size_t k_begin() const { return m_dimensions.ngc; }
    size_t q_begin() const { return 0; }

    size_t i_end() const { return m_dimensions.ngc + m_dimensions.nx; }
    size_t j_end() const { return m_dimensions.ngc + m_dimensions.ny; }
    size_t k_end() const { return m_dimensions.ngc + m_dimensions.nz; }
    size_t q_end() const { return m_dimensions.nq; }

    // all points loops
    size_t I_BEGIN() const { return 0; }
    size_t J_BEGIN() const { return 0; }
    size_t K_BEGIN() const { return 0; }
    size_t Q_BEGIN() const { return 0; }

    size_t I_END() const { return NX(); }
    size_t J_END() const { return NY(); }
    size_t K_END() const { return NZ(); }
    size_t Q_END() const { return NQ(); }

    void print_dimensions() const {

        std::cout << "NX: " << NX() << std::endl
                  << "NY: " << NY() << std::endl
                  << "NZ: " << NZ() << std::endl
                  << "NQ: " << NQ() << std::endl
                  << std::endl;
    }

    // accessors Array4d(i,j,k,q)
    inline double operator()(size_t i, size_t j, size_t k, size_t q) const {
        size_t idx = (i * m_NY * m_NZ * m_NQ) + (j * m_NZ * m_NQ) + (k * m_NQ) + q;
        return m_data[idx];
    }
    inline double& operator()(size_t i, size_t j, size_t k, size_t q) {
        size_t idx = (i * m_NY * m_NZ * m_NQ) + (j * m_NZ * m_NQ) + (k * m_NQ) + q;
        return m_data[idx];
    }

    MPI_Datatype x_direction_slicer() const;
    MPI_Datatype y_direction_slicer() const;
    MPI_Datatype z_direction_slicer() const;

    void set_zero();

protected:
    const Array4dDimensions m_dimensions;
    const size_t            m_NX, m_NY, m_NZ, m_NQ;

private:
    double* m_data;

    void allocate();
    void deallocate();

    int get_idx(size_t i, size_t j, size_t k, size_t q) const {
        size_t idx = (i * m_NY * m_NZ * m_NQ) + (j * m_NZ * m_NQ) + (k * m_NQ) + q;
        return idx;
    }

    size_t START() const { return 0; }
    size_t END() const { return NX() * NY() * NZ() * NQ(); }
};

#endif
