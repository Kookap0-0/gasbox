cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32 OR MINGW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

find_package(SFML COMPONENTS graphics window system REQUIRED)

# --- Добавляем исходники SFGraphing (убедитесь, что пути верные) ---
file(GLOB SFGRAPHING_SOURCES "libs/SFGraphing/src/*.cpp")

add_executable(gasbox
    src/main.cpp
    src/Box.cpp
    src/Simulation.cpp
    src/ParticleSoA.cpp
    ${SFGRAPHING_SOURCES}
)

target_link_libraries(gasbox
    sfml-graphics
    sfml-window
    sfml-system
)

target_include_directories(gasbox PRIVATE
    ${SFML_INCLUDE_DIR}
    "libs/SFGraphing/include"   # чтобы найти заголовки SFGraphing
)