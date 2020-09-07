#ifndef decomposition_hh
#define decomposition_hh

#include <mpi.h>
#include <vector>

#include "params.hpp"

class Decomposition {

public:
    Decomposition(){};

    Decomposition(const Params& p);

    ~Decomposition() = default;

    void cartesian_decomposition(const Params& p);

    std::vector<int> get_subdomain_node_count(const Params& p) const;

    std::vector<int> get_subdomain_start_idx(const Params& p) const;
    std::vector<int> get_subdomain_start_idx() const { return m_subdomain_start_indices; };

    std::vector<double> get_subdomain_start_coord(const Params& p) const;

    int get_subdomain_i() const { return m_block_kji[2]; }
    int get_subdomain_j() const { return m_block_kji[1]; }
    int get_subdomain_k() const { return m_block_kji[0]; }

    int get_rank() const { return m_process_rank; }

    MPI_Comm get_communicator() const { return m_comm; }

    bool east_physical() const { return m_comm_east_rank == MPI_PROC_NULL; }
    bool west_physical() const { return m_comm_west_rank == MPI_PROC_NULL; }
    bool south_physical() const { return m_comm_south_rank == MPI_PROC_NULL; }
    bool north_physical() const { return m_comm_north_rank == MPI_PROC_NULL; }
    bool front_physical() const { return m_comm_front_rank == MPI_PROC_NULL; }
    bool back_physical() const { return m_comm_back_rank == MPI_PROC_NULL; }

private:
    int m_process_rank; // my process rank

    // neighbouring process ranks
    int m_comm_west_rank, m_comm_east_rank;
    int m_comm_north_rank, m_comm_south_rank;
    int m_comm_front_rank, m_comm_back_rank;

    // number of domain splits in cartesian directions [ni,nj,nk]
    int m_num_domains[3];
    // indices of this block in the decomposed domain
    int m_block_kji[3];

    std::vector<int> m_subdomain_start_indices;

    int x_periodicity_check(const Params& p);
    int y_periodicity_check(const Params& p);
    int z_periodicity_check(const Params& p);

    ////////////////////////////////////////////////////////////
    ///
    ///		\brief 			Sets the neighbour ranks
    ///
    ////////////////////////////////////////////////////////////
    void set_neighbours();

    // mpi communicator handle
    MPI_Comm m_comm;
};

#endif