cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("ECS" C CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories("thirdparty/")
add_subdirectory("${CMAKE_SOURCE_DIR}/thirdparty/glad")
add_subdirectory("${CMAKE_SOURCE_DIR}/thirdparty/freetype")
include_directories("thirdparty/freetype/include/")

set(SOURCES src/main.cpp)

add_executable(ECS ${SOURCES})

target_link_libraries(ECS PRIVATE freetype glfw glad glm OpenGL::GL dl)

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/resources/shaders")
add_compile_definitions(SHADER_DIR="${SHADER_DIR}")
