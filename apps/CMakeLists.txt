set(Headers
)

set(Sources
    ./1d_euler.cpp
)


add_executable(OnedEuler ${Sources} ${Headers})


target_include_directories(OnedEuler PUBLIC
                            ${CMAKE_SOURCE_DIR}/src/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(OnedEuler PUBLIC 
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        io
                        )


set(Headers
)

set(Sources
    ./2d_euler.cpp
)


add_executable(TwodEuler ${Sources} ${Headers})
#set_target_properties(TwodEuler PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(TwodEuler PUBLIC
                            ${CMAKE_SOURCE_DIR}/src/
                            #${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(TwodEuler PUBLIC 
                        project_options
                        #OpenMP::OpenMP_CXX 
                        project_warnings
                        io
                        )