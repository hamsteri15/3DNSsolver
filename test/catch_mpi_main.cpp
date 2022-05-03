#define CATCH_CONFIG_RUNNER

#include <mpi.h>

#include "catch.hpp"


int main(int argc, char* argv[])
{

    MPI_Init(NULL, NULL);
    
    Catch::Session session;
  
    const int result = session.run(argc, argv);

    MPI_Finalize();
    return result;
    
    
}
