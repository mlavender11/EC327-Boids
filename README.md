### Project Description 📌
**Spherical Boids** is a high-performance 3D graphics engine and flocking simulation built from scratch in C++ and OpenGL. The simulation features thousands of boids dynamically orbiting a 3D textured Earth in real-time. By utilizing advanced graphics programming techniques like **Instanced Rendering**, **Custom GLSL Shaders**, and **Face Culling**, the engine renders 5,000+ individual boids alongside dynamic lighting, an atmosphere, and a day/night cycle. 

---

### Developers 🎓
- **Vugar Amirov** - *Computer Engineering*
- **Zhakhangir Mamayev** - *Computer Engineering*
- **Pree Simphliphan** - *Computer Engineering*
- **Rawisara Chairat** - *Computer Engineering*
- **Cole Shaigec** - *Electrical and Computer Engineering*

---

### Tech Stack ⚙️
- **Language:** C++17
- **Graphics API:** OpenGL 3.3 (Core Profile)
- **Libraries:** GLFW, GLAD, GLM, Dear ImGui, stb_image
- **Build System:** CMake

---

### Quick Start: Run the Simulation 🚀

#### 1. Mac Release (.app Bundle) - Recommended on Mac
If you have downloaded the pre-compiled `EC327-Boids.app` from the Releases tab:
1. **Unzip** the folder.
2. **Bypass Apple Gatekeeper:** Because this app is unsigned (we didn't pay Apple $99 so we aren't registered developers), macOS may flag it as damaged. Open **Terminal** and run:
   ```bash
   xattr -cr /path/to/EC327-Boids.app
   ```
3. **Launch:** Double-click the `.app` icon to run.

#### 2. Build from Source (Mac/Linux)
1. **Clone the Repo:**
   ```bash
   git clone https://github.com/your-username/EC327-Boids.git
   cd EC327-Boids
   ```
2. **Automatic Build Script:**
   ```bash
   chmod +x run.sh
   ./run.sh
   ```

#### 3. Manual CMake Build (Cross-Platform)
```bash
mkdir build && cd build
cmake ..
cmake --build . --parallel
cd ..
./build/EC327-Boids
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
│   ├── Application.cpp
│   ├── (Temporary boid generation files)
│   ├── behavior/
│   │   └── (Behavior things)
│   └── visual/
│       └── (Visual things)
└── CMakeLists.txt
```
---