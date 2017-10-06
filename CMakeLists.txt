cmake_minimum_required(VERSION 3.5)
project(realsense_demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(demo main.cpp)

target_link_libraries(demo pthread -lrealsense -lopencv_core -lopencv_highgui)
