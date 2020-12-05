#   Directory : Project Root
cmake_minimum_required(VERSION 3.8)

project(FLIGHT_CONTROLLER
    VERSION 0.2.0
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
add_subdirectory(test)