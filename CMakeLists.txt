cmake_minimum_required(VERSION 2.8)
project(cs111_hw4)
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(hw4 pa4.cpp)
target_link_libraries(hw4 ${OpenCV_LIBS})
