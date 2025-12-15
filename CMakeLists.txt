# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.27)

# Declare the name of the project
project(demo)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Ensure we are building for Intel macOS
set(CMAKE_OSX_ARCHITECTURES "x86_64")

# Include directories for GLAD, GLM, ASSIMP, and your own headers
include_directories(
        ${CMAKE_SOURCE_DIR}/glad/include
        include
        ${CMAKE_SOURCE_DIR}/glm-1.0.1/glm
        ${CMAKE_SOURCE_DIR}/assimp/include
)

# Add the GLAD source file
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

# Automatically find GLFW installed via Homebrew
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Define the executable target
add_executable(demo
        main.cpp
        src/shaderprogram.cpp
        src/stb_image.cpp
        src/mesh.cpp
        src/tiny_obj_loader.cpp
        src/camera.cpp
)

# Define paths for your assets and shaders
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")
add_compile_definitions(ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

# Link everything together
target_link_libraries(demo
        glad
        ${GLFW_LIBRARIES}
        "-framework OpenGL"
)