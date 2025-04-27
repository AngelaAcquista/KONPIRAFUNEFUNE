cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FINAL) ##add project name here replacing FINAL

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/Users/acqui/Downloads/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")
##set(SFML_DIR "zishu add ur root for sfml here")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(FINAL main.cpp) ##add project name here replacing FINAL

target_link_libraries(FINAL sfml-system sfml-window sfml-graphics sfml-audio) ## add project name here replacing FINAL
