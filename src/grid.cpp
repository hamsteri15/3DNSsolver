#include "grid.hpp"

Grid::Grid(const Params& p, const Decomposition& dec)
    : m_num_dimensions(p.dim)
    , m_process_rank(dec.get_rank())
    , m_step_sizes(compute_step_sizes(p))
    , m_start_coords(dec.get_subdomain_start_coord(p))
    , m_coords(create_point_coordinates(p, dec)) {
    // empty
}

std::vector<double> Grid::compute_step_sizes(const Params& p) {

    double              dx    = p.Lx / p.nx;
    double              dy    = p.Ly / p.ny;
    double              dz    = p.Lz / p.nz;
    std::vector<double> steps = {dx, dy, dz};
    return steps;
}

Array4d Grid::create_point_coordinates(const Params& p, const Decomposition& dec) {

    std::vector<int>    node_counts  = dec.get_subdomain_node_count(p);
    std::vector<double> start_coords = dec.get_subdomain_start_coord(p);

    int nx  = node_counts[0];
    int ny  = node_counts[1];
    int nz  = node_counts[2];
    int nq  = 3; //[x, y, z]
    int ngc = p.get_number_of_ghostcells();

    Array4d coords = Array4d(nx, ny, nz, nq, ngc);

    double x0 = start_coords[0];
    double y0 = start_coords[1];
    double z0 = start_coords[2];

    double dx = p.Lx / p.nx;
    double dy = p.Ly / p.ny;
    double dz = p.Lz / p.nz;

    double x, y, z;
    for (size_t i = coords.i_begin(); i < coords.i_end(); i++) {
        for (size_t j = coords.j_begin(); j < coords.j_end(); j++) {
            for (size_t k = coords.k_begin(); k < coords.k_end(); k++) {

                // boundary points at 1/2*delta from the boundary
                x = x0 + dx * 0.5 + (i - ngc) * dx;
                y = y0 + dy * 0.5 + (j - ngc) * dy;
                z = z0 + dz * 0.5 + (k - ngc) * dz;

                coords(i, j, k, 0) = x;
                coords(i, j, k, 1) = y;
                coords(i, j, k, 2) = z;
            }
        }
    }

    return coords;
}
