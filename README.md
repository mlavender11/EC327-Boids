***THE MAIN PROJECT DIRECTORY MUST BE CALLED "EC327-Boids"***

How to format stuff: https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax

# Folder Hierarchy
Important folders and files are listed here. Descriptions for folder contents are in parentheses.

```text
EC327-Boids/
├── build/
│   ├── EC327-Boids.exe
│   └── (Other stuff CMake needs)
├── external/
│   └── (A bunch of external libraries we use)
├── shaders/
│   └── (Shaders for boids and the earth)
├── src/
│   ├── main.cpp
│   ├── (Temporary boid generation files)
│   ├── behavior/
│   │   └── (Michael, Yicong, Ilias pls put stuff here)
│   └── visual/
│       └── (Boid rendering, camera movement, window gen, Earth model)
└── CMakeLists.txt
```

# How to Run
For the visual part, how you run it is a bit complicated because you need to use cmake

Here are the steps:

1. Go to the build folder (cd build)
2. Enter "cmake .." in the terminal
3. Enter "cmake --build ." in the terminal
4. Go to the main folder ("cd ..")
5. Enter "./build/EC327-Boids" (or whatever the .exe is called) in the terminal

# Behavior
Whatever you guys wanna put here

# Visuals
## BoidRenderer.h/cpp
These files have two important functions:

```std::vector<glm::mat4> BoidsToMatrices(const std::vector<Boids> &)```

```void DrawInstanced(const std::vector<glm::mat4> &)```

The first function, `BoidsToMatrices`, takes in a vector of boids objects, loops through each of them, and extracts each of their position and heading values (which need to be of type `glm::vec3`).

Those values are then added to a 4x4 matrix (of type `glm::mat4`). This is the most basic form so if we want to add colors or other characteristics that would need visualization, I would probably need to increase the size of this matrix.

It then returns a vector (of type `std::vector<glm::mat4>`) that contains the relevant information of all boids objects. 

The second function, `DrawInstanced`, draws all the boids from the vector returned by the first function. I did it this way because it's much more efficient than having each object draw itself like we did in PA3.

## Camera.h/cpp, Callbacks.h/cpp, and Window.h/cpp
These three modules handle the application setup and how the user interacts with the 3D world. 

**Window** contains `InitializeWindow`, which starts up GLFW, GLAD, and creates the OpenGL window. It also has `ProcessInput`, which is always waiting for the user to press escape to close the window (can add more things here as well--if we do want to add more inputs, keyboard inputs would be listened for here). 

**Camera** has `CalculateViewMatrix`, which converts the spherical coordinates of the camera into a matrix of cartesian coordinates. This matrix is sent to the shaders every frame so OpenGL knows where the camera is and what angle it is looking from. It also has the `CameraState` struct, which holds the following:
- The camera's spherical coordinates (radius, yaw, pitch)
- The camera's minimum and maximum zooms
- A bool of whether the mouse is being dragged
- The mouse's last coordinates on the screen

**Callbacks** intercepts mouse clicks, mouse movement, and scroll wheel actions. It uses these inputs to update `Camera.h`'s `CameraState` struct. It is apparently better to use callbacks for tracking mouse and scroll inputs. The bounds for the camera position and orientation are stored here.

## Earth.h/cpp
This class generates and renders the central planet that the boids fly around.

```Earth(float radius, int sectors, int stacks)```
The constructor procedurally generates a 3D sphere using math. The `sectors` and `stacks` parameters control the resolution (longitude and latitude lines).

```void Draw()```
During setup, the generated vertices are sent over to the GPU's memory using Vertex Buffer Objects (VBOs). The `Draw` function tells the GPU to render those pre-loaded buffers. It's okay that it has a generic name because it's `Earth::Draw()`.