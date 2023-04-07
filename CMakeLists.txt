cmake_minimum_required(VERSION 3.7)

project(snake-ai)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(snake-ai src/main.cpp src/Window.cpp src/Snake.cpp src/Pellet.cpp src/Game.cpp src/Utilities.cpp)

target_include_directories(snake-ai PRIVATE include/)
target_link_libraries(snake-ai ${SDL2_LIBRARIES})