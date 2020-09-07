SET(Headers
    ./array_3d_dimensions.hpp
    ./array_3d.hpp
    ./array_4d_dimensions.hpp
    ./array_4d.hpp
)


SET(Sources
    ./array_3d.cpp
    ./array_4d.cpp
)




ADD_LIBRARY(containers ${Sources} ${Headers})
TARGET_INCLUDE_DIRECTORIES(containers PUBLIC 
                            ${MPI_CXX_INCLUDE_PATH} 
                          )

TARGET_LINK_LIBRARIES(containers PUBLIC 
                        ${MPI_CXX_LIBRARIES} 
                        project_options
                        OpenMP::OpenMP_CXX 
                        #project_warnings
                        )