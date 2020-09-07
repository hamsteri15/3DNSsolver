SET(Headers
    ./diffusion_scheme.hpp
    ./centered_diffusion.hpp
)

SET(Sources
    ./diffusion_scheme.cpp
    ./centered_diffusion.cpp
)


ADD_LIBRARY(diffusion ${Sources} ${Headers})

TARGET_LINK_LIBRARIES(diffusion PUBLIC 
                        containers
                        diffop
                        project_options 
                        #project_warnings
                        )