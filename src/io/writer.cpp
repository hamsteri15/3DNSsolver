
#include "writer.hpp"

void Writer::print_4d_array_slice(const Array4d& arr, int component, int slice) {

    int nx = arr.I_END();
    int ny = arr.J_END();
    int nz = arr.K_END();

    double p;

    switch (slice) {

    // x-y plane
    case 1: {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {

                p = arr(i, j, (int)nz / 2, component);

                std::cout << std::setw(10) << p << "  ";
            }

            std::cout << std::endl;
        }

        break;
    }

    // x-z plane
    case 2: {
        for (int i = 0; i < nx; ++i) {
            for (int k = 0; k < nz; ++k) {

                p = arr(i, (int)ny / 2, k, component);
                std::cout << std::setw(10) << p << "  ";
            }

            std::cout << std::endl;
        }
        break;
    }

    // y-z plane
    case 3: {
        for (int j = 0; j < ny; ++j) {
            for (int k = 0; k < nz; ++k) {

                p = arr((int)nx / 2, j, k, component);

                std::cout << std::setw(10) << p << "  ";
            }

            std::cout << std::endl;
        }
        break;
    }

    default: {
        std::cout << "ERROR IN PRINTING 4d_array slice"; // error
        std::exit(1);
    }
    }
}