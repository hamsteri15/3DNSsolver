set(Headers
    ./write.hpp
    ./make_file.hpp
    ./make_datatype.hpp
    ./make_dataset.hpp
    ./writer.hpp
)

set(Sources
    ./write.cpp
    ./make_file.cpp
    ./make_datatype.cpp
    ./make_dataset.cpp
    ./writer.cpp
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
