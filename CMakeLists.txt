cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "C:/Users/kryst/CLionProjects/GRA/SDL_TTF")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(untitled main.cpp)


target_link_libraries(untitled ${SDL2_LIBRARY})
target_link_libraries(untitled "${SDL2_TTF_PATH}/SDL2_ttf.dll")
