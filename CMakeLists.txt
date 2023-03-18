cmake_minimum_required(VERSION 3.20)
set(project_name hotReloader)
project(${project_name})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${project_name} SHARED hr.cpp hr.hpp)
target_link_libraries(${project_name} PUBLIC stdc++fs)