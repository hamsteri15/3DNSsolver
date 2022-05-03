#Build libraries
set(libdirs 
    convection
)
SUBDIRS(${libdirs} )


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





