#   Directory : Project Root
cmake_minimum_required(VERSION 3.8)

project(FLIGHT_CONTROLLERS LANGUAGES CXX)
#set(CMAKE_CXX_COMPILER)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)