cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FINAL)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "Your path to SFML library here")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(FINAL main.cpp)
set_target_properties(FINAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries(FINAL
    PRIVATE
      # SFML
      sfml-system
      sfml-window
      sfml-graphics
      sfml-audio
)
