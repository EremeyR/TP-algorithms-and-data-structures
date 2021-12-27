cmake_minimum_required(VERSION 3.16)
project(module3)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES hw1.cpp src/ListGraph.cpp src/MatrixGraph.cpp src/SetGraph.cpp src/ArcGraph.cpp)
set(HEADER_FILES include/IGraph.h include/ListGraph.h include/MatrixGraph.h include/SetGraph.h include/ArcGraph.h)


add_executable(${PROJECT_NAME}_hw1 ${SOURCES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME}_hw1 PUBLIC include)