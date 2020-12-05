#   Directory : Project Root
cmake_minimum_required(VERSION 3.19)

project(
    FLIGHT_CONTROLLER
    LANGUAGES CXX
    )
set(CMAKE_CXX_COMPILER)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)