# Created by Yuxi Luo, May 1, 2018

cmake_minimum_required(VERSION 3.6)
project(Game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_MIXER REQUIRED)

# setup include directory
include_directories(src/* src/glm ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# compile all the cpp files and link to `game` library
file(GLOB game_sources src/*/*.cpp src/*/*.h src/setUp.cpp src/*.h)
set(game ${game_sources})
add_library(game ${game})

add_executable(main src/main.cpp)
target_link_libraries(main game ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ./)
