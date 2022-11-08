set(Headers
    ./equation.hpp
    ./euler_primitive_variables.hpp
    ./euler.hpp
    ./euler_flux.hpp
    ./equation_of_state.hpp
    ./flux.hpp
)

set(Sources
    ./equation.cpp
    ./euler_primitive_variables.cpp
    ./euler.cpp
    ./euler_flux.cpp
    ./flux.cpp
)


add_library(equation ${Sources} ${Headers})

target_include_directories(equation PUBLIC
                            ${CMAKE_SOURCE_DIR}/external/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(equation PUBLIC
                        #containers
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        )
