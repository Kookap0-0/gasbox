cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# --- ImGui ---
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui")

# --- ImPlot ---
set(IMPLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/implot")

# ImGui-SFML
add_subdirectory(libs/imgui-sfml)

add_executable(gasbox
    src/main.cpp
    src/Box.cpp
    src/Simulation.cpp
    src/ParticleSoA.cpp

    # ImGui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    # ImPlot ✅ ОБЯЗАТЕЛЬНО
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

target_include_directories(gasbox PRIVATE
    ${IMGUI_DIR}
    ${IMPLOT_DIR}
)

target_link_libraries(gasbox PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML
)