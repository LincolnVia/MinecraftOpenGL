cmake_minimum_required(VERSION 3.15)
project(Voxel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Include folders
include_directories(include src)

# Add executable
add_executable(voxel
    main.cpp
    src/Engine/Engine.cpp
    src/glad.c
    src/basicVerts.cpp
    
    src/World/World.cpp
)

# Copy shaders folder to build
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets

)

# Link libraries
target_link_libraries(voxel OpenGL::GL dl glfw)

# Optional: put executable in a bin folder inside build
set_target_properties(voxel PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/
)
