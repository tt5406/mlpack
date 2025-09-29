cmake_minimum_required(VERSION 3.16)
project(mlpack_rl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Allow reuse of mlpack dependency discovery helpers.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../CMake")
include(mlpack)

option(RLPACK_DOWNLOAD_DEPENDENCIES "Automatically download third-party dependencies required by the reinforcement learning subset." ON)
option(RLPACK_REQUIRE_DEPENDENCIES "Fail configuration if mandatory third-party dependencies are unavailable." ON)
option(RLPACK_ENABLE_SMOKE_TEST "Build a small executable that validates the reinforcement learning headers." ON)

if (RLPACK_DOWNLOAD_DEPENDENCIES)
  set(MLPACK_DONT_FIND_MLPACK ON)
  set(BLAS_FOUND ON CACHE BOOL "" FORCE)
  fetch_mlpack(FALSE)
  find_armadillo(${CMAKE_BINARY_DIR})
  find_ensmallen(${CMAKE_BINARY_DIR})
  find_cereal(${CMAKE_BINARY_DIR})
else()
  find_armadillo()
  find_ensmallen()
  find_cereal()
endif()

set(_rlpack_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(_rlpack_link_libraries)

if (ARMADILLO_FOUND)
  list(APPEND _rlpack_include_dirs ${ARMADILLO_INCLUDE_DIRS})
  list(APPEND _rlpack_link_libraries ${ARMADILLO_LIBRARIES})
endif()
if (ENSMALLEN_FOUND)
  list(APPEND _rlpack_include_dirs ${ENSMALLEN_INCLUDE_DIR})
endif()
if (CEREAL_FOUND)
  list(APPEND _rlpack_include_dirs ${CEREAL_INCLUDE_DIR})
endif()

if (RLPACK_REQUIRE_DEPENDENCIES)
  if (NOT ARMADILLO_FOUND)
    message(FATAL_ERROR "Armadillo was not found; it is required to build the reinforcement learning subset.")
  endif()
  if (NOT ENSMALLEN_FOUND)
    message(FATAL_ERROR "Ensmallen was not found; it is required to build the reinforcement learning subset.")
  endif()
  if (NOT CEREAL_FOUND)
    message(FATAL_ERROR "cereal was not found; it is required to build the reinforcement learning subset.")
  endif()
endif()

if (RLPACK_REQUIRE_DEPENDENCIES)
  find_package(Boost 1.58 REQUIRED COMPONENTS filesystem program_options serialization system)
else()
  find_package(Boost 1.58 QUIET COMPONENTS filesystem program_options serialization system)
endif()

if (Boost_FOUND)
  list(APPEND _rlpack_include_dirs ${Boost_INCLUDE_DIRS})
  list(APPEND _rlpack_link_libraries ${Boost_LIBRARIES})
elseif (RLPACK_REQUIRE_DEPENDENCIES)
  message(FATAL_ERROR "Boost (>=1.58) was not found; it is required to build the reinforcement learning subset.")
endif()

if (RLPACK_ENABLE_SMOKE_TEST AND ARMADILLO_FOUND AND ENSMALLEN_FOUND AND CEREAL_FOUND AND Boost_FOUND)
  add_executable(mlpack_rl_example src/rl_dummy.cpp)
  target_include_directories(mlpack_rl_example PRIVATE ${_rlpack_include_dirs})
  target_link_libraries(mlpack_rl_example PRIVATE ${_rlpack_link_libraries})
endif()

add_library(mlpack_rl INTERFACE)
target_include_directories(mlpack_rl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../src>
  $<INSTALL_INTERFACE:include>
  ${_rlpack_include_dirs}
)
target_link_libraries(mlpack_rl INTERFACE ${_rlpack_link_libraries})

install(DIRECTORY ../src/mlpack/methods/reinforcement_learning
        DESTINATION include/mlpack/methods)
install(DIRECTORY ../src/mlpack/methods/ann
        DESTINATION include/mlpack/methods)
install(DIRECTORY ../src/mlpack/core
        DESTINATION include/mlpack)
install(FILES ../src/mlpack/methods/reinforcement_learning.hpp
              ../src/mlpack/core.hpp
              ../src/mlpack/base.hpp
              ../src/mlpack/prereqs.hpp
              ../src/mlpack/config.hpp
        DESTINATION include/mlpack)
