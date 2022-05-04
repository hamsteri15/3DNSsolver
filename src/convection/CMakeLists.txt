set(Headers
    ./convection_flux.hpp
)

set(Sources
    ./convection_flux.cpp
)


add_library(convection ${Sources} ${Headers})

target_include_directories(convection PUBLIC
                            ${CMAKE_SOURCE_DIR}/external/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(convection PUBLIC 
                        #containers
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        )
