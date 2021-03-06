CMAKE_MINIMUM_REQUIRED(VERSION 3.12)


PROJECT(FlowSolver)

message("COMPILER ID " ${CMAKE_CXX_COMPILER_ID})
INCLUDE(cmake/StandardProjectSettings.cmake)


message("BINARY DIR" ${CMAKE_BINARY_DIR})



# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_11)
#target_compile_features(project_options INTERFACE cxx_std_20)


#Global compiler flags

set(OptimizationFlags
    -pedantic 
    -msse3 
    -mavx 
    -mfpmath=sse 
    -O3
    -ffast-math 
    -march=native 
    -ftree-vectorize 
)

add_compile_options(${OptimizationFlags})






# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)


#Configure MPI
FIND_PACKAGE(MPI REQUIRED)
MESSAGE("STATUS MPI FOUND: ${MPI_CXX_FOUND}")


#Configure HDF5
#SET(HDF5_FIND_DEBUG true)
SET(HDF5_PREFER_PARALLEL true)
FIND_PACKAGE(HDF5 REQUIRED)
MESSAGE(STATUS "Found HDF5: ${HDF5_FOUND}")
MESSAGE(STATUS "HDF5 is parallel: ${HDF5_IS_PARALLEL}")
MESSAGE(STATUS "Prefer parallel: ${HDF5_PREFER_PARALLEL}")


option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

#Configure OpenMP
FIND_PACKAGE(OpenMP)



#Set library and binary paths
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)




    

#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/containers)
#Build solvers
INCLUDE_DIRECTORIES(src)
ADD_SUBDIRECTORY(src)




