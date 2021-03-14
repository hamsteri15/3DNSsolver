#ifndef Grid_hh
#define Grid_hh
#include "containers/array_4d.hpp"
#include "decomposition.hpp"
#include "params.hpp"

class Grid {

public:
    Grid() = default;

    Grid(const Params& p, const Decomposition& dec);

    ~Grid() = default;

    double dx() const { return m_step_sizes[0]; }
    double dy() const { return m_step_sizes[1]; }
    double dz() const { return m_step_sizes[2]; }

    std::vector<double> get_step_sizes() const { return m_step_sizes; }

    const Array4d& get_coords() const { return m_coords; }

    size_t get_rank() const { return m_process_rank; }

    // Array& get_coords() {return *(this->m_coords); }

    int is_3D() const { return m_num_dimensions == 3; }

private:
    size_t              m_num_dimensions; // 1D, 2D or 3D
    size_t              m_process_rank;
    std::vector<double> m_step_sizes;   // dx dy dz
    std::vector<double> m_start_coords; // x0 y0 z0

    Array4d m_coords;

    std::vector<double> compute_step_sizes(const Params& p);
    Array4d             create_point_coordinates(const Params& p, const Decomposition& dec);
};

#endif
