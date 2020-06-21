cmake_minimum_required(VERSION 3.17)
project(comp_geo)

set(CMAKE_CXX_STANDARD 20)

add_executable(clustering k_clustering.cpp Stopwatch.hpp)
