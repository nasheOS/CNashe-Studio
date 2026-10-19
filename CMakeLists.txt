cmake_minimum_required(VERSION 3.16)
project(CNasheStudio LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE QUIET IMPORTED_TARGET SDL2_image)

add_executable(cnashe_studio
    src/main.cpp
    src/Engine/Scene.cpp
    src/Engine/Runtime.cpp
    src/Editor/Editor.cpp
)
target_include_directories(cnashe_studio PRIVATE src)
target_link_libraries(cnashe_studio PRIVATE PkgConfig::SDL2)
if(SDL2_IMAGE_FOUND)
    target_link_libraries(cnashe_studio PRIVATE PkgConfig::SDL2_IMAGE)
    target_compile_definitions(cnashe_studio PRIVATE CNASHE_HAS_SDL_IMAGE=1)
endif()
if(WIN32)
    target_link_libraries(cnashe_studio PRIVATE mingw32)
endif()
