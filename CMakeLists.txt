cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# СТАТИЧНЫЙ MINGW RUNTIME
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

find_package(SFML COMPONENTS graphics window system REQUIRED)

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