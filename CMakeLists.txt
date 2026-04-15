cmake_minimum_required(VERSION 3.10)
project(EC327-Boids)

set(CMAKE_CXX_STANDARD 17)

# Find GLFW and OpenGL
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    src
    src/visual
    src/behavior
    external/glad/include
    external
    external/imgui
    shaders
)

# Collect all ImGui source files
file(GLOB IMGUI_SOURCES 
    "external/imgui/*.cpp" 
    "external/imgui/backends/imgui_impl_glfw.cpp" 
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

# Define the executable and its source files
add_executable(EC327-Boids 
    src/main.cpp 
    src/Application.cpp

    src/visual/GraphicsEngine.cpp
    src/visual/Callbacks.cpp 
    src/visual/Camera.cpp
    src/visual/Window.cpp
    src/visual/BoidRenderer.cpp
    shaders/Shader.cpp
    src/visual/CelestialBody.cpp
    src/visual/Star.cpp

    # include behavior files when they work

    src/BoidGenTemp.cpp

    external/glad/src/glad.c 
    ${IMGUI_SOURCES}
)

# Link the libraries to executable
target_link_libraries(EC327-Boids 
    glfw 
    OpenGL::GL 
)