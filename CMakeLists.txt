cmake_minimum_required(VERSION 3.12)
add_compile_options(-O3)
project(big_integer)

set(CMAKE_CXX_STANDARD 17)

add_library(big_integer big_integer.cpp big_integer.h transformer.cpp transformer.h)
add_executable(temp temp.cpp transformer.h transformer.cpp big_integer.cpp big_integer.h)
target_link_libraries(temp big_integer)

