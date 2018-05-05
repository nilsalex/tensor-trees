cmake_minimum_required(VERSION 3.9)

project(tensor-trees)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-int-in-bool-context")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(EIGEN_INCLUDE /usr/include/eigen3)

include_directories(${CMAKE_SOURCE_DIR}/include ${EIGEN_INCLUDE})
add_library(tensor-trees SHARED src/Node src/Tensor src/Epsilon src/Eta src/Scalar src/Algorithms src/Builder src/LinearAlgebra)
add_executable(main src/main)
add_executable(main2 src/main2)
target_link_libraries(main tensor-trees gmp gmpxx)
target_link_libraries(main2 tensor-trees gmp gmpxx)
