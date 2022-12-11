set(Headers
    ./equation.hpp
    ./euler_primitive_variables.hpp
    ./euler.hpp
    ./euler_flux.hpp
    ./equation_of_state.hpp
    ./flux.hpp
    ./boundary_mirror.hpp
)

set(Sources
    ./equation.cpp
    ./euler_primitive_variables.cpp
    ./euler.cpp
    ./euler_flux.cpp
    ./equation_of_state.cpp
    ./flux.cpp
    ./boundary_mirror.cpp
)


add_library(equation ${Sources} ${Headers})

target_link_libraries(equation PUBLIC
                        #containers
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        )
