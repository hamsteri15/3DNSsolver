#ifndef Communicator_hh
#define Communicator_hh

#include "./containers/array_3d.hpp"
#include "./containers/array_4d.hpp"
#include "decomposition.hpp"
#include "params.hpp"
#include <iostream>
#include <mpi.h>

class Communicator {

public:
    Communicator() = default;

    Communicator(const Decomposition& decomposition_info);

    Communicator(const Array4d& arr, const Decomposition& decomposition_info);
    Communicator(const Array3d& arr, const Decomposition& decomposition_info);

    void communicate(Array4d& arr) const;

    MPI_Comm get_communicator() const { return m_cartesian_comm; };

private:
    MPI_Comm m_cartesian_comm; // Cartesian communicator topology

    int m_global_dims[3]; //[k_max, j_max, i_max]

    // my local coordinates relative to the global domain
    int m_local_coords[3]; //[k, j, i]

    /// REMOVE
    int m_nx, m_ny, m_nz, m_ngc;

    void communicate_x_direction(Array4d& arr) const;
    void communicate_y_direction(Array4d& arr) const;
    void communicate_z_direction(Array4d& arr) const;

protected:
};

#endif
