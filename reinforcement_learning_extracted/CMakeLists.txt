cmake_minimum_required(VERSION 3.11)
project(mlpack_rl C CXX)

# Set a default build type if none was specified.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Include necessary CMake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(mlpack)

# Options.
option(USE_OPENMP "If available, use OpenMP for parallelization." ON)
option(DOWNLOAD_DEPENDENCIES "Automatically download dependencies if not available." ON)

# Find Armadillo.
set(MLPACK_DONT_FIND_MLPACK 1)
if(DOWNLOAD_DEPENDENCIES)
  fetch_mlpack(ON)
else()
  find_mlpack()
endif()
unset(MLPACK_DONT_FIND_MLPACK)

# Set include directories.
include_directories(BEFORE ${ARMADILLO_INCLUDE_DIRS})
include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}")

# On Windows, we need to set the output directories.
if (WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
endif()

# The reinforcement learning library is header-only, so we don't need to build
# any sources.  We will, however, define an interface library so that users can
# easily link against it.
add_library(mlpack_rl INTERFACE)
target_include_directories(mlpack_rl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(mlpack_rl INTERFACE ${ARMADILLO_LIBRARIES})

# Add the test executable.
add_executable(test_rl main.cpp)
target_link_libraries(test_rl PRIVATE mlpack_rl)

# Installation rules.
install(DIRECTORY reinforcement_learning/
    DESTINATION include/mlpack/methods/reinforcement_learning)
install(DIRECTORY mlpack/
    DESTINATION include/mlpack)
install(FILES mlpack.hpp
    DESTINATION include)

# Install the library target.
install(TARGETS mlpack_rl EXPORT mlpack_rl-targets)
install(EXPORT mlpack_rl-targets
    FILE mlpack_rl-targets.cmake
    NAMESPACE mlpack::
    DESTINATION lib/cmake/mlpack_rl)