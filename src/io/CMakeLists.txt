SET(Headers
    ./hdf5_writer.hpp
    ./io_binary.hpp
    ./output_info.hpp
    ./reader.hpp
    ./tinyxml2.hpp
    ./writer.hpp
)

SET(Sources
    ./hdf5_writer.cpp
    ./io_binary.cpp
    ./output_info.cpp
    ./reader.cpp
    ./tinyxml2.cpp
    ./writer.cpp
)



ADD_LIBRARY(io ${Sources} ${Headers})
TARGET_INCLUDE_DIRECTORIES(io PUBLIC 
                            ${HDF5_INCLUDE_DIRS}
                            ${MPI_CXX_INCLUDE_PATH} 
                            )

TARGET_LINK_LIBRARIES(io 
                    containers 
                    #project_warnings 
                    project_options
                    ${HDF5_LIBRARIES} 
                    ${MPI_CXX_LIBRARIES})
