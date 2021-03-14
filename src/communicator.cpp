#include "communicator.hpp"

Communicator::Communicator(const Decomposition& decomposition_info)
    : m_cartesian_comm(decomposition_info.get_communicator()) {}

void Communicator::communicate(Array4d& arr) const {

    communicate_x_direction(arr);
    // MPI_Barrier(m_cartesian_comm);
    communicate_y_direction(arr);
    // MPI_Barrier(m_cartesian_comm);
    communicate_z_direction(arr);
    // MPI_Barrier(m_cartesian_comm);
}

void Communicator::communicate_x_direction(Array4d& arr) const {

    size_t ngc = arr.ngc();
    size_t nx  = arr.nx();

    MPI_Datatype slice = arr.x_direction_slicer();

    int source, dest;
    ////////////////////////////////////////
    // east to east (positive x-dir)
    MPI_Cart_shift(m_cartesian_comm, 2, 1, &source, &dest);
    MPI_Sendrecv(&arr(nx, 0, 0, 0),
                 1,
                 slice,
                 dest,
                 0,
                 &arr(0, 0, 0, 0),
                 1,
                 slice,
                 source,
                 0,
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    ////////////////////////////////////////
    // west to west (negative x-dir)
    MPI_Cart_shift(m_cartesian_comm, 2, -1, &source, &dest);
    MPI_Sendrecv(&arr(ngc, 0, 0, 0),
                 1,
                 slice,
                 dest,
                 0,
                 &arr(ngc + nx, 0, 0, 0),
                 1,
                 slice,
                 source,
                 0,
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    MPI_Type_free(&slice);
}

void Communicator::communicate_y_direction(Array4d& arr) const {

    size_t ngc = arr.ngc();
    size_t ny  = arr.ny();

    MPI_Datatype slice = arr.y_direction_slicer();

    int source, dest;
    ////////////////////////////////////////
    // south to south (positive y-dir)
    MPI_Cart_shift(m_cartesian_comm, 1, 1, &source, &dest);
    MPI_Sendrecv(&arr(0, ny, 0, 0),
                 1,
                 slice,
                 dest,
                 0, // take from south domain indices
                 &arr(0, 0, 0, 0),
                 1,
                 slice,
                 source,
                 0, // place to north halo indices
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    ////////////////////////////////////////
    // north to north (negative y-dir)
    MPI_Cart_shift(m_cartesian_comm, 1, -1, &source, &dest);
    MPI_Sendrecv(&arr(0, ngc, 0, 0),
                 1,
                 slice,
                 dest,
                 0, // take from north domain indices
                 &arr(0, ngc + ny, 0, 0),
                 1,
                 slice,
                 source,
                 0, // place to south halo indices
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    MPI_Type_free(&slice);
}

void Communicator::communicate_z_direction(Array4d& arr) const {

    size_t ngc = arr.ngc();
    size_t nz  = arr.nz();

    int source, dest;

    MPI_Datatype slice = arr.z_direction_slicer();

    ////////////////////////////////////////
    // front to front (positive z-direction)
    MPI_Cart_shift(m_cartesian_comm, 0, 1, &source, &dest);
    MPI_Sendrecv(&arr(0, 0, nz, 0),
                 1,
                 slice,
                 dest,
                 0, // take from front domain indices
                 &arr(0, 0, 0, 0),
                 1,
                 slice,
                 source,
                 0, // place to back halo indices
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    ////////////////////////////////////////
    // back to back (negative z-direction)
    MPI_Cart_shift(m_cartesian_comm, 0, -1, &source, &dest);
    MPI_Sendrecv(&arr(0, 0, ngc, 0),
                 1,
                 slice,
                 dest,
                 0, // take from back domain indices
                 &arr(0, 0, ngc + nz, 0),
                 1,
                 slice,
                 source,
                 0, // place to front halo indices
                 m_cartesian_comm,
                 MPI_STATUS_IGNORE);

    MPI_Type_free(&slice);
}
