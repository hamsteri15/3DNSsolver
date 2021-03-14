#include "params.hpp"

void Params::sanity_check() const {

    check_testcase_validity();
    check_convection_scheme_validity();
    check_diffusion_scheme_validity();
    check_boundary_validity();
    check_writer_validity();
}

void Params::check_convection_scheme_validity() const {

    // Check that the convection scheme is valid
    std::vector<std::string> valid_convection_schemes = {"Weno", "Centered", "none"};

    if (item_in_vector(this->convection_scheme, valid_convection_schemes)) {

        std::cout << " Using convection scheme: " << this->convection_scheme << std::endl;
        std::vector<int> valid_convection_orders;

        if (this->convection_scheme == "Weno") {
            valid_convection_orders = {5};
            check_weight_validity();
        }

        else {
            // centered checks
            valid_convection_orders = {2, 4, 6};
        }

        if (item_in_vector(this->convection_scheme_order, valid_convection_orders)) {
            std::cout << " Convection scheme order: " << this->convection_scheme_order << std::endl;
        } else {
            std::cout << "Convection scheme order: " << this->convection_scheme_order
                      << " is not valid. Exiting." << std::endl;
            std::exit(1);
        }

        std::cout << " Convection in characteristic space? yes/no (0/1): "
                  << this->convection_in_characteristic_space << std::endl;
    }

    else {
        std::cout << "Convection scheme: " << this->convection_scheme << " is not valid. Exiting."
                  << std::endl;
        std::exit(1);
    }
}

void Params::check_testcase_validity() const {

    // Check that the testcase is valid
    std::vector<std::string> valid_test_cases = {"random",
                                                 "sod_x",
                                                 "sod_y",
                                                 "sod_z",
                                                 "case3",
                                                 "case4",
                                                 "case6",
                                                 "case12",
                                                 "case15",
                                                 "case17",
                                                 "shear_xy",
                                                 "jet",
                                                 "rank",
                                                 "shear_3d"};

    if (item_in_vector(this->testcase, valid_test_cases)) {
        std::cout << "Running test case: " << this->testcase << std::endl;
    } else {
        std::cout << "Testcase: " << this->testcase << " is not valid. Exiting." << std::endl;
        std::exit(1);
    }
}

void Params::check_weight_validity() const {

    // Check that the Weno weight type is valid
    std::vector<std::string> valid_weights = {"Shu", "Borges"};

    if (item_in_vector(this->weight_type, valid_weights)) {
        std::cout << " With weight type: " << this->weight_type << std::endl;
    } else {
        std::cout << "Weight type: " << this->weight_type << " is not valid. Exiting." << std::endl;
        std::exit(1);
    }
}

void Params::check_diffusion_scheme_validity() const {

    // Check that the diffusion scheme is valid
    std::vector<std::string> valid_diffusion_schemes = {"Centered", "Shen", "none"};

    if (item_in_vector(this->diffusion_scheme, valid_diffusion_schemes)) {
        std::cout << " Using diffusion scheme: " << this->diffusion_scheme << std::endl;
    } else {
        std::cout << "Diffusion scheme: " << this->diffusion_scheme << " is not valid. Exiting."
                  << std::endl;
        std::exit(1);
    }

    std::vector<int> valid_diffusion_orders = {2, 4, 6};

    if (item_in_vector(this->diffusion_scheme_order, valid_diffusion_orders)) {
        std::cout << " Diffusion scheme order: " << this->diffusion_scheme_order << std::endl;
    } else {
        std::cout << "Diffusion scheme order: " << this->diffusion_scheme_order
                  << " is not valid. Exiting." << std::endl;
        std::exit(1);
    }
}

void Params::check_boundary_validity() const {

    std::vector<std::string> valid_boundary_conditions = {"Periodic", "Neumann", "Inflow"};

    std::cout << "Boundary conditions: " << std::endl;
    for (auto const& iter : this->boundary_conditions) {

        if (item_in_vector(iter.second, valid_boundary_conditions)) {
            std::cout << iter.first << " " << iter.second << "\n";

        }

        else {
            std::cout << "Boundary condition: " << iter.second << " is not valid (at " << iter.first
                      << "). Exiting." << std::endl;
            std::exit(1);
        }
    }
}

void Params::check_writer_validity() const {

    std::vector<std::string> valid_output_formats = {"Hdf5", "Binary"};

    if (item_in_vector(this->output_file_format, valid_output_formats)) {
        std::cout << " Output file format: " << this->output_file_format << std::endl;
    }

    else {
        std::cout << "Output file format: " << this->output_file_format << " is not valid. Exiting."
                  << std::endl;
        std::exit(1);
    }
}

template <typename T> int Params::item_in_vector(T item, std::vector<T> v) const {

    if (std::find(v.begin(), v.end(), item) != v.end()) return 1;

    return 0;
}
