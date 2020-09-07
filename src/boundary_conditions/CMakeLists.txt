SET(Headers
    ./boundary.hpp
    ./boundary_neumann.hpp
)


SET(Sources
    ./boundary.cpp
    ./boundary_neumann.cpp
)

ADD_LIBRARY(boundary ${Sources} ${Headers})

TARGET_LINK_LIBRARIES(boundary PRIVATE 
                        containers 
                        project_options
                        OpenMP::OpenMP_CXX 
                        #project_warnings
                        )