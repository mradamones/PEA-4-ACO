cmake_minimum_required(VERSION 3.24)
project(ant)

set(CMAKE_CXX_STANDARD 17)

add_executable(ant main.cpp ant_colony.cpp ant_colony.h ant.cpp ant.h)
