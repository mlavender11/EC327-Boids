***THE MAIN PROJECT DIRECTORY MUST BE CALLED "EC327-Boids"***

# Directory Architecture
"->" means it's a folder
"=>" means it's files

EC327-Boids/
->  build/
    => Stuff for CMake, as well as the executable for the program
->  external/
    => Downloaded external libraries
->  shaders/
    => Shader files
->  src/
    ->  behavior/
        => Stuff to determine how the boids move and interact, as well as the environment (eventually)
    ->  visual/
        => Stuff to render everything
    => main.cpp

# How to Run
For the visual part, how you run it is a bit complicated because you need to use cmake

Here are the steps:

1. Go to the build folder (cd build)
2. Enter "cmake .." in the terminal
3. Enter "cmake --build ." in the terminal
4. Go to the main folder ("cd ..")
5. Enter "./build/EC327-Boids" (or whatever the .exe is called) in the terminal

# Behavior
Michael and Yicong whatever you guys wanna put here

# Visuals
...