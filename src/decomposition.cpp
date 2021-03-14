#include "decomposition.hpp"

Decomposition::Decomposition(const Params& p) {

    m_num_domains[0] = p.nproc_x;
    m_num_domains[1] = p.nproc_y;
    m_num_domains[2] = p.nproc_z;

    cartesian_decomposition(p);
    set_neighbours();
    m_subdomain_start_indices = get_subdomain_start_idx(p);
}

void Decomposition::set_neighbours() {

    // west and east neighbours
    MPI_Cart_shift(m_comm, 2, 1, &m_comm_west_rank, &m_comm_east_rank);

    // north and south neighbours
    MPI_Cart_shift(m_comm, 1, 1, &m_comm_north_rank, &m_comm_south_rank);

    // front and back neighbours
    MPI_Cart_shift(m_comm, 0, 1, &m_comm_back_rank, &m_comm_front_rank);
}

std::vector<int> Decomposition::get_subdomain_node_count(const Params& p) const {

    int total_nx = p.nx;
    int total_ny = p.ny;
    int total_nz = p.nz;

    int block_i = get_subdomain_i();
    int block_j = get_subdomain_j();
    int block_k = get_subdomain_k();

    int num_domains_i = p.nproc_x;
    int num_domains_j = p.nproc_y;
    int num_domains_k = p.nproc_z;

    // should truncate if not equal division
    int local_nx = total_nx / num_domains_i;
    int local_ny = total_ny / num_domains_j;
    int local_nz = total_nz / num_domains_k;

    int remainder_i = total_nx % num_domains_i;
    int remainder_j = total_ny % num_domains_j;
    int remainder_k = total_nz % num_domains_k;

    // add the reminder to the first block rows
    if (block_i == 0) { local_nx += remainder_i; }
    if (block_j == 0) { local_ny += remainder_j; }
    if (block_k == 0) { local_nz += remainder_k; }

    std::vector<int> node_count = {local_nx, local_ny, local_nz};

    return node_count;
}

std::vector<int> Decomposition::get_subdomain_start_idx(const Params& p) const {

    int total_nx = p.nx;
    int total_ny = p.ny;
    int total_nz = p.nz;

    int block_i = get_subdomain_i();
    int block_j = get_subdomain_j();
    int block_k = get_subdomain_k();

    int num_domains_i = p.nproc_x;
    int num_domains_j = p.nproc_y;
    int num_domains_k = p.nproc_z;

    // should truncate if not equal division
    int local_nx = int(total_nx / num_domains_i);
    int local_ny = int(total_ny / num_domains_j);
    int local_nz = int(total_nz / num_domains_k);

    int remainder_i = total_nx % num_domains_i;
    int remainder_j = total_ny % num_domains_j;
    int remainder_k = total_nz % num_domains_k;

    int start_idx_i = block_i * local_nx;
    int start_idx_j = block_j * local_ny;
    int start_idx_k = block_k * local_nz;

    if (block_i > 0) { start_idx_i += remainder_i; }
    if (block_j > 0) { start_idx_j += remainder_j; }
    if (block_k > 0) { start_idx_k += remainder_k; }

    std::vector<int> start_idx = {start_idx_i, start_idx_j, start_idx_k};

    return start_idx;
}

std::vector<double> Decomposition::get_subdomain_start_coord(const Params& p) const {

    std::vector<int> start_idx = get_subdomain_start_idx(p);

    int start_i = start_idx[0];
    int start_j = start_idx[1];
    int start_k = start_idx[2];

    double dx = p.Lx / p.nx;
    double dy = p.Ly / p.ny;
    double dz = p.Lz / p.nz;

    double start_c_x = dx * start_i;
    double start_c_y = dy * start_j;
    double start_c_z = dz * start_k;

    std::vector<double> start_coords = {start_c_x, start_c_y, start_c_z};

    return start_coords;
}

void Decomposition::cartesian_decomposition(const Params& p) {

    int periodicity_info[3] = {
        z_periodicity_check(p), y_periodicity_check(p), x_periodicity_check(p)};

    // note the order here is different than in m_num_domains because mpi_cart_shift
    // assumes certain topology
    int num_processes[3] = {p.nproc_z, p.nproc_y, p.nproc_x};

    // always reoder for efficiency
    int reorder = 1;
    // number of spatial dimensions is always three
    int num_spatial_dims = 3;

    MPI_Cart_create(MPI_COMM_WORLD,   // old handle
                    num_spatial_dims, // number of cartesian dimensions (always three)
                    num_processes,    // number of processes in each direction
                    periodicity_info, // periodic or not in each direction
                    reorder,          // always reorder
                    &m_comm           // new handle
    );

    // get my rank
    MPI_Comm_rank(m_comm, &m_process_rank);

    // get my coordinates
    MPI_Cart_coords(m_comm, m_process_rank, 3, m_block_kji);
}

int Decomposition::x_periodicity_check(const Params& p) {

    auto search = p.boundary_conditions.find("West");
    return (search->second == "Periodic");
}

int Decomposition::y_periodicity_check(const Params& p) {

    auto search = p.boundary_conditions.find("South");
    return (search->second == "Periodic");
}

int Decomposition::z_periodicity_check(const Params& p) {

    auto search = p.boundary_conditions.find("Front");
    return (search->second == "Periodic");
}
