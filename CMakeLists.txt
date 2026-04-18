cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(gasbox
    src/main.cpp
    src/Particle.cpp
    src/Box.cpp
    src/Simulation.cpp
)

target_link_libraries(gasbox
    sfml-graphics
    sfml-window
    sfml-system
)