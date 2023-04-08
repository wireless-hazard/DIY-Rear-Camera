cmake_minimum_required(VERSION 3.7)

project(time_lapse)
add_compile_options(-Wall -Wextra -pedantic -Werror -fsized-deallocation -Wno-exceptions -Wold-style-cast -Wuseless-cast -O3 -std=c++17)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(webcam_sampler ../webcam_sampler/webcam.cpp)
target_include_directories(webcam_sampler PUBLIC ../webcam_sampler/)
target_link_libraries(webcam_sampler ${OpenCV_LIBS})

add_executable(time_lapse main.cpp)
target_link_libraries(time_lapse webcam_sampler)