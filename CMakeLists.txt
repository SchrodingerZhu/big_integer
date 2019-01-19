cmake_minimum_required(VERSION 3.12)
project(big_integer)

set(CMAKE_CXX_STANDARD 17)

add_library(transformer transformer.cpp transformer.h)
add_library(big_integer STATIC big_integer.cpp big_integer.h)
link_libraries(transformer)
add_executable(temp temp.cpp transformer.h transformer.cpp)
target_link_libraries(temp transformer)

