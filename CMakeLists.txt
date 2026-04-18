cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(SFML_RUNTIME_DIR /home/reyn/libs/SFML-2.6.1/bin)

if (WIN32)
    add_custom_command(TARGET gasbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_RUNTIME_DIR}
        $<TARGET_FILE_DIR:gasbox>
    )
endif()