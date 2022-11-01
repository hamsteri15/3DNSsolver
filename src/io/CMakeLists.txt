set(Headers
    ./make_datatype.hpp
    ./writer.hpp
    ./reader.hpp
    ./write_xdmf.hpp
)

set(Sources
    ./make_datatype.cpp
    ./writer.cpp
    ./reader.cpp
    ./write_xdmf.cpp
)


add_library(io ${Sources} ${Headers})

target_include_directories(io PUBLIC
                            ${CMAKE_SOURCE_DIR}/external/
                          )

#The SYSTEM prevents warnings from H5/MPI headers
target_include_directories(io SYSTEM PUBLIC
                            ${HDF5_INCLUDE_DIRS}
                            ${MPI_CXX_INCLUDE_PATH}
                          )

target_link_libraries(io PUBLIC
                        equation
                        #containers
                        project_options
                        ${HDF5_LIBRARIES}
                        ${MPI_CXX_LIBRARIES}
                        #OpenMP::OpenMP_CXX 
                        #project_warnings
                        )
