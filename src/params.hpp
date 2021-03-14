#ifndef params_hh
#define params_hh

#include <algorithm> //std::find
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "error_messages.hpp"

class Params {
public:
    void sanity_check() const;

    size_t get_number_of_ghostcells() const {
        return 3;
    } // this should be changed to adjust based on discretization

    int dim,
        io_interval, // interval on when to output
        nx, ny,
        nz,      // number of ghost cells
        nproc_x, // number of processes x-dir
        nproc_y, nproc_z;

    int ngc = 3;

    double Lx, Ly, Lz,
        mu,      // dynamic viscosity
        Pr,      // Prandtl number
        gamma,   // ratio of specific heats
        CFL_max, // maximum convective Courant number
        end_t;   // end time for simulation

    std::string testcase, riemann_solver, convection_scheme, weight_type, diffusion_scheme,
        boundary_type, output_file_format;

    int convection_scheme_order;
    int convection_in_characteristic_space;
    int diffusion_scheme_order;

    std::map<std::string, std::string> boundary_conditions;

private:
    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			These functions check that the
    ///						user-inputs are valid
    ///
    ///		\param p		Global parameters placeholder
    ///
    ////////////////////////////////////////////////////////////
    void check_convection_scheme_validity() const;
    void check_weight_validity() const;
    void check_diffusion_scheme_validity() const;
    void check_testcase_validity() const;
    void check_boundary_validity() const;
    void check_writer_validity() const;

    ////////////////////////////////////////////////////////////
    ///
    ///    	\brief 			Checks if item is in vector
    ///
    ///		\param item		Item to check from
    ///		\param v		vector
    ///
    ////////////////////////////////////////////////////////////
    template <typename T> int item_in_vector(T item, std::vector<T> v) const;
};

#endif
