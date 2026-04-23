# Install script for directory: /home/reyn/repos/gasbox/libs/imgui-sfml

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE STATIC_LIBRARY FILES "/home/reyn/repos/gasbox/build-linux/libs/imgui-sfml/libImGui-SFML.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/reyn/repos/gasbox/libs/imgui-sfml/imgui-SFML.h"
    "/home/reyn/repos/gasbox/libs/imgui-sfml/imgui-SFML_export.h"
    "/home/reyn/repos/gasbox/libs/imgui/imconfig.h"
    "/home/reyn/repos/gasbox/libs/imgui/imgui.h"
    "/home/reyn/repos/gasbox/libs/imgui/imgui_internal.h"
    "/home/reyn/repos/gasbox/libs/imgui/imstb_rectpack.h"
    "/home/reyn/repos/gasbox/libs/imgui/imstb_textedit.h"
    "/home/reyn/repos/gasbox/libs/imgui/imstb_truetype.h"
    "/home/reyn/repos/gasbox/libs/imgui/misc/cpp/imgui_stdlib.h"
    "/home/reyn/repos/gasbox/libs/imgui-sfml/imconfig-SFML.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML/ImGui-SFML.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML/ImGui-SFML.cmake"
         "/home/reyn/repos/gasbox/build-linux/libs/imgui-sfml/CMakeFiles/Export/3d81db8e277c760ab8b6bed07c34c449/ImGui-SFML.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML/ImGui-SFML-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML/ImGui-SFML.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML" TYPE FILE FILES "/home/reyn/repos/gasbox/build-linux/libs/imgui-sfml/CMakeFiles/Export/3d81db8e277c760ab8b6bed07c34c449/ImGui-SFML.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML" TYPE FILE FILES "/home/reyn/repos/gasbox/build-linux/libs/imgui-sfml/CMakeFiles/Export/3d81db8e277c760ab8b6bed07c34c449/ImGui-SFML-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/ImGui-SFML" TYPE FILE FILES "/home/reyn/repos/gasbox/libs/imgui-sfml/cmake/ImGui-SFMLConfig.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/reyn/repos/gasbox/build-linux/libs/imgui-sfml/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
