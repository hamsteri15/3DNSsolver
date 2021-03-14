#ifndef Array3d_hh
#define Array3d_hh

#include "array_3d_dimensions.hpp"
#include "array_4d_dimensions.hpp"
#include <iostream>
#include <mpi.h>
#include <stdlib.h>

class Array3d {

public:
    Array3d();
    Array3d(const Array3dDimensions& dimensions);

    // from 4d array dimensions, the fourth dimension neglected
    Array3d(const Array4dDimensions& dimensions);

    Array3d(const size_t nx, const size_t ny, const size_t nz, const size_t nbarriers);

    ~Array3d();

    const Array3dDimensions& get_dimensions() const { return m_dimensions; }

    size_t nx() const { return m_dimensions.nx; }
    size_t ny() const { return m_dimensions.ny; }
    size_t nz() const { return m_dimensions.nz; }
    size_t ngc() const { return m_dimensions.ngc; }

    size_t NX() const { return m_dimensions.NX; }
    size_t NY() const { return m_dimensions.NY; }
    size_t NZ() const { return m_dimensions.NZ; }

    // inner points loops
    size_t i_begin() const { return m_dimensions.ngc; }
    size_t j_begin() const { return m_dimensions.ngc; }
    size_t k_begin() const { return m_dimensions.ngc; }

    size_t i_end() const { return m_dimensions.ngc + m_dimensions.nx; }
    size_t j_end() const { return m_dimensions.ngc + m_dimensions.ny; }
    size_t k_end() const { return m_dimensions.ngc + m_dimensions.nz; }

    // all points loops
    size_t I_BEGIN() const { return 0; }
    size_t J_BEGIN() const { return 0; }
    size_t K_BEGIN() const { return 0; }

    size_t I_END() const { return NX(); }
    size_t J_END() const { return NY(); }
    size_t K_END() const { return NZ(); }

    void print_dimensions() const {

        std::cout << "NX: " << NX() << std::endl
                  << "NY: " << NY() << std::endl
                  << "NZ: " << NZ() << std::endl
                  << std::endl;
    }

    // accessors Array4d(i,j,k,q)
    inline double operator()(size_t i, size_t j, size_t k) const {
        size_t idx = (i * m_NY * m_NZ) + (j * m_NZ) + k;
        return m_data[idx];
    }
    inline double& operator()(size_t i, size_t j, size_t k) {
        size_t idx = (i * m_NY * m_NZ) + (j * m_NZ) + k;
        return m_data[idx];
    }

    // MPI_Datatype x_direction_slicer() const;
    // MPI_Datatype y_direction_slicer() const;
    // MPI_Datatype z_direction_slicer() const;

protected:
    const Array3dDimensions m_dimensions;
    const size_t            m_NX, m_NY, m_NZ;

private:
    double* m_data;

    void allocate();
    void deallocate();

    int get_idx(size_t i, size_t j, size_t k) const {
        size_t idx = (i * m_NY * m_NZ) + (j * m_NZ) + k;
        return idx;
    }
};

#endif