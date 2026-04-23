cmake_minimum_required(VERSION 3.16)
project(gasbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Путь к ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui")

# Подключаем ImGui-SFML (он найдёт ImGui через IMGUI_DIR)
add_subdirectory(libs/imgui-sfml)

# Добавляем в исполняемый файл все ваши исходники И файлы ImGui
add_executable(gasbox
    src/main.cpp
    src/Box.cpp
    src/Simulation.cpp
    src/ParticleSoA.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Добавляем пути для поиска заголовков
target_include_directories(gasbox PRIVATE
    ${IMGUI_DIR}
    libs/imgui-sfml
)

target_link_libraries(gasbox PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML   # это цель из imgui-sfml
)