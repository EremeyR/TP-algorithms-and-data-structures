cmake_minimum_required(VERSION 3.16)
project(module_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}_1 hw1.cpp)
add_executable(${PROJECT_NAME}_2 hw2.cpp)

add_executable(${PROJECT_NAME}_4 hw4.cpp)

add_executable(${PROJECT_NAME}_test test.cpp)