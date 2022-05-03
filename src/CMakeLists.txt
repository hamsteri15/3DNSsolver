
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/libs)


#Build the app
SET(AppHeaders
#    ./communicator.hpp
#    ./decomposition.hpp
#    ./domain.hpp
#    ./driver.hpp
#    ./eq_of_state.hpp
#    ./error_messages.hpp
#    ./grid.hpp
#    ./matrix.hpp
#    ./params.hpp
#    ./testcases.hpp
#    ./time_scheme.hpp
#    ./timer.hpp
#    ./viscosity.hpp
)

SET(AppSources
    ./main.cpp
#    ./communicator.cpp
#    ./decomposition.cpp
#    ./domain.cpp
#    ./driver.cpp
#    ./eq_of_state.cpp
#    #./error_messages.cpp
#    ./grid.cpp
#    ./matrix.cpp
#    ./params.cpp
#    ./testcases.cpp
#    ./time_scheme.cpp
#    ./timer.cpp
#    ./viscosity.cpp
)

ADD_EXECUTABLE(FlowSolver ${AppSources} ${AppHeaders})


#TARGET_INCLUDE_DIRECTORIES(FlowSolver PUBLIC 
#                            ${MPI_CXX_INCLUDE_PATH} 
#                          )


#TARGET_LINK_LIBRARIES(FlowSolver PUBLIC 
#                        containers
#                        boundary
#                        diffop 
#                        convection
#                        diffusion 
#                        io
#                        ${MPI_CXX_LIBRARIES}
#                        OpenMP::OpenMP_CXX)





