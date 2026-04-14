***THE MAIN PROJECT DIRECTORY MUST BE CALLED "EC327-Boids"***
https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax

# Directory Architecture
**Important folders and files are listed here**
Descriptions for folder contents are in *italics*.

EC327-Boids\
├── build\
│   ├── EC327-Boids.exe\
│   └── *Other stuff CMake needs*\
├── external\
│   └── *A bunch of external libraries we use*\
├── shaders\
│   └── *Shaders for boids and the earth*\
├── src\
│   ├── main.cpp\
│   ├── *Temporary boid generation files*\
│   ├── behavior\
│   │   └── **Michael and Yicong pls put stuff here**\
│   ├── visual\
│   │   └── *Boid rendering, camera movement, window gen, Earth model*\
└── CMakeLists.txt

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
**BoidRenderer.h, BoidRenderer.cpp:**

These files have two important functions:

> `std::vector<glm::mat4> BoidsToMatrices(const std::vector<Boids> &)`

> `void DrawInstanced(const std::vector<glm::mat4> &)`

The first function, `BoidsToMatrices`, takes in a vector of boids objects, loops through each of them, and extracts each of their position and heading values (which need to be of type `glm::vec3`).

Those values are then added to a 4x4 matrix (of type `glm::mat4`). This is the most basic form so if we want to add colors or other characteristics that would need visualization, I would probably need to increase the size of this matrix.

It then returns a vector (of type `std::vector<glm::mat4>`) that contains the relevant information of all boids objects. 

The second function, `DrawInstanced`, draws all the boids from the vector returned by the first function. I did it this way because it's much more efficient than having each object draw itself like we did in PA3.