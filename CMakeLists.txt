cmake_minimum_required(VERSION 3.7)

project(rear_view)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(webcam_sampler main.cpp)
target_link_libraries(webcam_sampler ${OpenCV_LIBS})