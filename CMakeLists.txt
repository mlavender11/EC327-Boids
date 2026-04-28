cmake_minimum_required(VERSION 3.10)
project(EC327-Boids)

if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Download and build GLFW from source
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4 # Use the latest stable release
)

# Turn off GLFW settings we don't need to speed up compile time
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# FORCE STATIC LINKING - No DLLs required!
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(glfw)

# Include directories
include_directories(
    src
    src/visual
    src/behavior
    external/glad/include
    external
    external/imgui
    external/imgui/backends
    external/stb
    shaders
    assets
)

# Collect all ImGui source files
file(GLOB IMGUI_SOURCES 
    "external/imgui/*.cpp" 
    "external/imgui/backends/imgui_impl_glfw.cpp" 
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

# Define the executable and its source files
# CMake ignores the MACOSX_BUNDLE flag on non-macOS platforms, so it's safe
add_executable(EC327-Boids MACOSX_BUNDLE
    src/main.cpp 
    src/Application.cpp
    src/PathResolver.cpp

    src/visual/GraphicsEngine.cpp
    src/visual/UIManager.cpp
    src/visual/Callbacks.cpp 
    src/visual/Camera.cpp
    src/visual/Window.cpp
    src/visual/BoidRenderer.cpp
    src/visual/CelestialBody.cpp
    src/visual/Star.cpp
    src/visual/TextureLoader.cpp

    src/behavior/Boids.cpp
    src/behavior/Friendly.cpp
    src/behavior/Predator.cpp
    src/behavior/Flock.cpp

    shaders/Shader.cpp

    # External libraries
    external/glad/src/glad.c
    external/stb/stb_image.cpp
    ${IMGUI_SOURCES}
)

# Check if compiling on a Mac
if(APPLE)
    set_target_properties(EC327-Boids PROPERTIES MACOSX_BUNDLE TRUE)
    
    add_custom_command(TARGET EC327-Boids POST_BUILD
        # 1. Copy Assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_BUNDLE_DIR:EC327-Boids>/Contents/Resources/assets"
        # 2. Copy Shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
            "$<TARGET_BUNDLE_DIR:EC327-Boids>/Contents/Resources/shaders"
    )
else()
    # For Windows and Linux
    add_custom_command(TARGET EC327-Boids POST_BUILD
        # 1. Copy Assets next to the .exe
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:EC327-Boids>/assets"
        # 2. Copy Shaders next to the .exe
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:EC327-Boids>/shaders"
    )
endif()

# Link the libraries to executable
target_link_libraries(EC327-Boids 
    glfw
    OpenGL::GL
)