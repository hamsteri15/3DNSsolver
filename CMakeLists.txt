CMAKE_MINIMUM_REQUIRED(VERSION 3.12)


PROJECT(FlowSolver)

message("COMPILER ID " ${CMAKE_CXX_COMPILER_ID})
INCLUDE(cmake/StandardProjectSettings.cmake)


message("BINARY DIR" ${CMAKE_BINARY_DIR})



# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
#target_compile_features(project_options INTERFACE cxx_std_20)









# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)


#Configure MPI
find_package(MPI REQUIRED)
message("STATUS MPI FOUND: ${MPI_CXX_FOUND}")


#Configure HDF5
set(HDF5_FIND_DEBUG true)
set(HDF5_PREFER_PARALLEL true)
find_package(HDF5 REQUIRED)
message(STATUS "Found HDF5: ${HDF5_FOUND}")
message(STATUS "HDF5 is parallel: ${HDF5_IS_PARALLEL}")
message(STATUS "Prefer parallel: ${HDF5_PREFER_PARALLEL}")


option(ENABLE_TESTING "Enable Test Builds" ON)

#Configure OpenMP
FIND_PACKAGE(OpenMP)



#Set library and binary paths
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)




    

#Build sources
INCLUDE_DIRECTORIES(src)
ADD_SUBDIRECTORY(src)

#Build apps
#INCLUDE_DIRECTORIES(apps)
#ADD_SUBDIRECTORY(apps)

if(ENABLE_TESTING)
  enable_testing()
  message(
    "Building Tests."
  )
  add_subdirectory(test)
endif()


