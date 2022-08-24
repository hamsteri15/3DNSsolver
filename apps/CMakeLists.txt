set(Headers
)

set(Sources
    ./1d_euler.cpp
)


add_executable(OnedEuler ${Sources} ${Headers})


target_include_directories(OnedEuler PUBLIC
                            ${CMAKE_SOURCE_DIR}/external/
                            ${CMAKE_SOURCE_DIR}/src/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(OnedEuler PUBLIC 
                        convection
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        )
