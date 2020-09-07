SET(Headers
    ./centered_convection.hpp
    ./convection_constants.hpp
    ./convection_flux.hpp
    ./convection_scheme.hpp
    ./weno-5.hpp
    ./weno-5cw.hpp
)

SET(Sources
    ./centered_convection.cpp
    #./convection_constants.cpp
    ./convection_flux.cpp
    ./convection_scheme.cpp
    ./weno-5.cpp
    ./weno-5cw.cpp
)


ADD_LIBRARY(convection ${Sources} ${Headers})
#TARGET_INCLUDE_DIRECTORIES(containers PUBLIC 
#                            ${MPI_CXX_INCLUDE_PATH} 
#                          )

TARGET_LINK_LIBRARIES(convection PUBLIC 
                        containers
                        project_options
                        OpenMP::OpenMP_CXX 
                        #project_warnings
                        )