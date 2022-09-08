set(Headers
    ./equation.hpp
    ./euler.hpp
    ./euler_flux.hpp
    ./grid.hpp
    ./cartesian_grid.hpp
    ./volumetric_field.hpp
    ./surface_field.hpp
    ./equation_of_state.hpp
)

set(Sources
    ./equation.cpp
    ./euler.cpp
    ./euler_flux.cpp
    ./grid.cpp
    ./cartesian_grid.cpp
    ./volumetric_field.cpp
    ./surface_field.hpp
    ./equation_of_state.cpp
)


add_library(equation ${Sources} ${Headers})

target_include_directories(equation PUBLIC
                            ${CMAKE_SOURCE_DIR}/external/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(equation PUBLIC
                        convection
                        #containers
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        )
