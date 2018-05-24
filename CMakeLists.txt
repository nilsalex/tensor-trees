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

find_package(Boost COMPONENTS serialization REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${EIGEN_INCLUDE})
add_library(tensor-trees SHARED src/Node src/Tensor src/Epsilon src/Eta src/Scalar src/Algorithms src/Indices src/Builder src/LinearAlgebra)
add_executable(main_area_2nd src/main_area_2nd)
add_executable(main_area_2nd_mass src/main_area_2nd_mass)
add_executable(main_area_1st src/main_area_1st)
add_executable(main_area_1st_mass src/main_area_1st_mass)
add_executable(main_metric_1st src/main_metric_1st)
add_executable(main_metric_2nd src/main_metric_2nd)
target_link_libraries(main_area_2nd tensor-trees gmp gmpxx ${Boost_LIBRARIES})
target_link_libraries(main_area_2nd_mass tensor-trees gmp gmpxx ${Boost_LIBRARIES})
target_link_libraries(main_area_1st tensor-trees gmp gmpxx ${Boost_LIBRARIES})
target_link_libraries(main_area_1st_mass tensor-trees gmp gmpxx ${Boost_LIBRARIES})
target_link_libraries(main_metric_1st tensor-trees gmp gmpxx ${Boost_LIBRARIES})
target_link_libraries(main_metric_2nd tensor-trees gmp gmpxx ${Boost_LIBRARIES})
