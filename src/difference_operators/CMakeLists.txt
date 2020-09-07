SET(Headers
    ./cd_constants.hpp
    ./cd-n.hpp
    ./difference_operator.hpp
)

SET(Sources
    ./cd-n.cpp
)


ADD_LIBRARY(diffop ${Sources} ${Headers})


TARGET_LINK_LIBRARIES(diffop PUBLIC 
                        containers
                        project_options
                        OpenMP::OpenMP_CXX 
                        #project_warnings
                        )