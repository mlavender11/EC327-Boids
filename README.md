### Project Description 📌
We built a high-performance 3D graphics engine and flocking simulation from scratch in C++ and OpenGL. The simulation features (up to) thousands of boids dynamically orbiting Earth in real-time. **EXPLAIN WHAT BOIDS ARE IN A LOT OF DETAIL FOR DUMMIES**


By utilizing advanced graphics programming techniques like Instanced Rendering, Custom GLSL Shaders, and Face Culling, the engine renders 5,000+ individual boids alongside dynamic lighting, an atmosphere, and a day/night cycle. 

---

### Developers 🎓
- **Kyle Armstrong** - *Biomedical Engineering*
- **Michael ...** - *... Engineering*
- **Ilias ...** - *... Engineering*
- **Yicong ...** - *... Engineering*

---

### Tech Stack ⚙️
- **Language:** C++17
- **Graphics API:** OpenGL 3.3 (Core Profile)
- **Libraries:** GLFW, GLAD, GLM, Dear ImGui, stb_image
- **Build System:** CMake

---

### Quick Start: Run the Simulation 🚀

#### 1. Mac Release (.app Bundle) - Recommended on Mac
1. Download `EC327-Boids-Mac.zip` from the latest release.
2. Unzip the folder if your Mac doesn't automatically unzip it.
3. Bypass Apple Gatekeeper: Because this app is unsigned (we didn't pay Apple $99 so we aren't registered developers), macOS may flag it as damaged. Open Terminal and run:
   ```bash
   xattr -cr /path/to/EC327-Boids.app
   ```
4. Double-click the `.app` icon to run.

#### 2. Windows/Linux Release - Recommended on Windows/Linux
1. Download `EC327-Boids-Windows.zip` or `EC327-Boids-Linux.zip`, depending on your platform, from the latest release.
2. **Unzip** the folder.
3. In the folder, double-click the `.exe` file to run.

#### 3. Build from Source (Mac/Linux)
1. **Clone the Repo:**
   ```bash
   git clone https://github.com/mlavender11/EC327-Boids.git
   cd EC327-Boids
   ```
2. **Automatic Build Script:**
   ```bash
   chmod +x run.sh
   ./run.sh
   ```

---

### Software Architecture & Features 🖥️

#### Visual Engine
- **Custom GLSL Shaders:** Features separate shader programs for the Earth, Atmosphere, Star (Sun), and Boids, supporting Blinn-Phong lighting and transparency blending.
- **Instanced Rendering:** Utilizes matrix instancing to pass transformation data for 5,000+ boids to the GPU in a single draw call, maintaining high frame rates.
- **Dynamic Lighting & Day/Night Cycle:** A light-source "Sun" orbits the Earth, dynamically updating the lighting vectors for all objects in the scene.
- **Atmospheric Scattering:** A secondary celestial sphere with alpha blending and back-face culling to simulate a glowing atmospheric shell.

#### Behavior & Simulation
- **Spherical Boids Algorithm:** An implementation of Reynolds' Flocking Algorithm (Cohesion, Separation, and Alignment) constrained to a spherical orbital volume.
- **Live Parameter Tuning:** Integrated **Dear ImGui** overlay allows for real-time adjustment of boid speed, visual range, and steering forces.
- **Dynamic Camera:** A custom camera controller with zoom-clamping to prevent clipping into the Earth's surface or wandering too far into space.

#### Logistics & Framework
- **Smart Path Resolution:** Includes a custom `PathResolver` utility that uses Apple's `_NSGetExecutablePath` to automatically locate shaders and textures whether the app is run from a terminal, a build folder, or a `.app` bundle.
- **State Management:** A robust `Application` state machine handling `SETUP`, `SIMULATION`, and `PAUSED` states.
- **Post-Build Automation:** CMake is configured to automatically package assets and shaders into the Apple bundle `Resources` directory during compilation.

---

### Dependencies [Required for source builds] 🗝️
- **CMake** (3.12+)
- **OpenGL-compatible Hardware**
- **C++17 Compiler** (Clang/GCC/MSVC)
- **GLFW** (System-level installation recommended on Mac: `brew install glfw`)

---

### Key Controls ⌨️
- **Mouse Drag:** Rotate camera around the Earth
- **Scroll:** Zoom in/out (Clamped to Atmosphere/Surface)
- **Escape:** Toggle Pause/Menu
- **UI Sliders:** Adjust Boid behavior in real-time

---

### Gallery / Demo 📺
*(Add screenshots or a YouTube link here)*

---

### Software Architecture Diagrams 🗺️
*(Add Image/Diagram links here, e.g., Class Diagrams or Shader Pipelines)*

#### File Diagram
Important folders and files are listed here. Descriptions for folder contents are in parentheses.
```text
EC327-Boids/
├── assets/
│   ├── fonts/
│   │   └── (Fonts)
│   └── (Texture images)
├── build/
│   └── (CMake stuff - this folder is deleted and re-made each run)
├── external/
│   └── (A bunch of external libraries we use)
├── shaders/
│   └── (Shaders for all rendered objects)
├── src/
│   ├── main.cpp
│   ├── Application.cpp
│   ├── PathResolver.cpp
│   ├── behavior/
│   │   └── (Behavior things)
│   └── visual/
│       └── (Visual things)
└── CMakeLists.txt
```

#### The Shader Pipeline

A shader pipeline (or Graphics Pipeline) is the sequence of mathematical and logical steps the GPU takes to turn 3D geometric data into the 2D pixels displayed on your screen. In this project, we utilize a programmable pipeline via GLSL (OpenGL Shading Language) to control everything from the positioning of boids to the glow of the atmosphere.

**1. Vertex Processing (Shaping the World)**

The pipeline begins by receiving raw vertex data—such as 3D coordinates, colors, and texture positions—from the CPU.

Transformation: The Vertex Shader transforms 3D coordinates into "Clip Space" using Matrix math (Model, View, and Projection matrices).

Instancing: To render 5,000+ boids, we inject an instanceMatrix at this stage, allowing the GPU to calculate the unique position and orientation of every boid simultaneously.

**2. Rasterization (Geometry to Pixels)**

Once vertices are positioned, the GPU assembles them into triangles.

Face Culling: To optimize performance, the pipeline discards "back-facing" triangles that are hidden from the camera's view.

Interpolation: The GPU determines which screen pixels are covered by a triangle and mathematically "blends" data (like texture coordinates) across the surface.

**3. Fragment Processing (Color & Light)**

This stage determines the final color of every individual pixel (fragment).

Texturing: The Fragment Shader samples colors from the Earth texture map based on the interpolated coordinates.

Lighting Calculations: We use the Sun's lightDir to calculate how light hits each surface, creating realistic highlights on the boids and a day/night cycle on the planet.

**4. Testing & Blending (The Final Pass)**

Before a pixel is drawn, the GPU performs final checks to ensure visual consistency:

Depth Testing: The pipeline compares the depth of a new pixel against the "Depth Buffer" to ensure that boids in front of the Earth are visible, while those behind it are hidden.

Alpha Blending: For transparent objects like the Atmosphere, the pipeline blends the fragment color with the existing background colors using specific blend functions.

---