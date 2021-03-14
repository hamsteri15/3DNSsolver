#include "driver.hpp"
#include <iostream>

int main() {

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    Driver driver = Driver();

    driver.run();

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}
