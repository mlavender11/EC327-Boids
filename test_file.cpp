/**
 * test_file.cpp
 *
 * Tests the Boids simulation on a globe surface.
 *
 * Phase 1 (0–10 sec): Boids flock normally around the globe using the
 *   standard separation, alignment, and cohesion rules.
 *
 * Phase 2 (10 sec+): Every boid receives a "turn left" steering force
 *   each frame. "Left" is computed relative to each boid's current
 *   velocity and the outward normal of the globe at its position, so
 *   the turn is always tangential to the sphere surface.
 *
 * Compile (from the project root, after CMake setup):
 *   g++ -std=c++17 -I external/glm test_file.cpp \
 *       src/behavior/Boids.cpp src/behavior/Flock.cpp \
 *       src/behavior/Friendly.cpp src/BoidGenTemp.cpp \
 *       -o test_boids
 *
 * Or add it to CMakeLists.txt as an additional executable target.
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/behavior/Boids.h"
#include "src/behavior/Flock.h"
#include "src/BoidGenTemp.h"

// ---------------------------------------------------------------------------
// Simulation parameters
// ---------------------------------------------------------------------------
static constexpr int   NUM_BOIDS        = 50;     // Number of boids to simulate
static constexpr float GLOBE_RADIUS     = 10.0f;  // Matches configEarthRadius in Application.h
static constexpr float MIN_ALTITUDE     = 10.0f;  // Same defaults as Application
static constexpr float MAX_ALTITUDE     = 15.0f;

static constexpr float TURN_LEFT_FORCE  = 0.4f;   // Magnitude of the left-turn steering force
static constexpr float PHASE_SWITCH_SEC = 10.0f;  // When to start turning left (seconds)
static constexpr float SIM_END_SEC      = 20.0f;  // Total simulation duration (seconds)
static constexpr float DT               = 0.016f; // ~60 fps fixed timestep (seconds)

// ---------------------------------------------------------------------------
// Helper: compute a "left" unit vector for a boid on the globe surface.
//
// For a boid moving with velocity `vel` at position `pos`:
//   - The outward globe normal is normalize(pos).
//   - The boid's forward direction is normalize(vel).
//   - "Left" (port side) = cross(up, forward), where up = globe normal.
//
// This keeps the turn tangential to the sphere so boids don't fly off.
// ---------------------------------------------------------------------------
static glm::vec3 leftTurnVector(const glm::vec3 &pos, const glm::vec3 &vel)
{
    if (glm::length(vel) < 0.001f)
        return glm::vec3(0.0f);

    glm::vec3 up      = glm::normalize(pos);
    glm::vec3 forward = glm::normalize(vel);
    glm::vec3 left    = glm::cross(up, forward); // perpendicular, pointing left

    if (glm::length(left) < 0.001f)
        return glm::vec3(0.0f); // velocity parallel to up (edge case)

    return glm::normalize(left);
}

// ---------------------------------------------------------------------------
// Helper: print a brief status line to stdout for a sample boid.
// ---------------------------------------------------------------------------
static void printStatus(float t, const Boids &sample, bool turningLeft)
{
    glm::vec3 p = sample.getPosition();
    glm::vec3 v = sample.getVelocity();
    float altitude = glm::length(p) - GLOBE_RADIUS;

    std::cout << std::fixed << std::setprecision(2)
              << "t=" << std::setw(6) << t << "s  "
              << (turningLeft ? "[TURNING LEFT] " : "[FLOCKING]     ")
              << "boid#0  pos=("
              << p.x << "," << p.y << "," << p.z << ")  "
              << "alt=" << altitude << "  "
              << "speed=" << glm::length(v) << "\n";
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main()
{
    std::cout << "=== Boids Globe Test ===\n";
    std::cout << "Boids: " << NUM_BOIDS << "  |  Globe radius: " << GLOBE_RADIUS << "\n";
    std::cout << "Phase 1 (0-" << PHASE_SWITCH_SEC << "s): normal flocking\n";
    std::cout << "Phase 2 (" << PHASE_SWITCH_SEC << "-" << SIM_END_SEC << "s): turn left\n\n";

    // -------------------------------------------------------------------
    // 1. Spawn boids on the globe using GenerateTestBoids for positions,
    //    then construct actual Boids objects from those positions.
    //    GenerateTestBoids gives us transformation matrices; we extract
    //    the translation column (column 3) for each boid's start position
    //    and column 2 for its forward direction to seed the velocity.
    // -------------------------------------------------------------------
    std::vector<glm::mat4> initTransforms =
        GenerateTestBoids(NUM_BOIDS, MIN_ALTITUDE, MAX_ALTITUDE);

    // Build a flat list of Boids* so we can call flock() ourselves and
    // also inject the turn-left force when the time comes.
    std::vector<Boids *> boidList;
    boidList.reserve(NUM_BOIDS);

    for (const glm::mat4 &m : initTransforms)
    {
        glm::vec3 pos      = glm::vec3(m[3]);          // translation column
        glm::vec3 forward  = glm::normalize(glm::vec3(m[2])); // Z column = forward

        // Give each boid a meaningful starting velocity (1.5 units/s forward)
        glm::vec3 initVel  = forward * 1.5f;

        boidList.push_back(new Boids(pos.x, pos.y, pos.z, initVel));
    }

    std::cout << "Spawned " << boidList.size() << " boids.\n\n";

    // -------------------------------------------------------------------
    // 2. Simulation loop
    // -------------------------------------------------------------------
    float simTime    = 0.0f;
    bool  turningLeft = false;
    int   printEvery  = static_cast<int>(0.5f / DT); // print every ~0.5 s
    int   frameCount  = 0;

    while (simTime < SIM_END_SEC)
    {
        // --- Phase transition check ---
        if (!turningLeft && simTime >= PHASE_SWITCH_SEC)
        {
            turningLeft = true;
            std::cout << "\n>>> 10 seconds reached — all boids are now turning LEFT <<<\n\n";
        }

        // --- Gather neighbors for every boid (mirrors Flock::update) ---
        std::vector<std::vector<const Boids *>> allNeighbors;
        allNeighbors.reserve(boidList.size());

        for (const Boids *b : boidList)
            allNeighbors.push_back(b->findNeighbors(boidList));

        // --- Apply flocking forces ---
        for (size_t i = 0; i < boidList.size(); i++)
            boidList[i]->flock(allNeighbors[i]);

        // --- Apply turn-left force (Phase 2 only) ---
        if (turningLeft)
        {
            for (Boids *b : boidList)
            {
                glm::vec3 leftForce =
                    leftTurnVector(b->getPosition(), b->getVelocity()) * TURN_LEFT_FORCE;
                b->applyForce(leftForce);
            }
        }

        // --- Step all boids forward ---
        for (Boids *b : boidList)
            b->update(DT);

        // --- Periodic console output ---
        if (frameCount % printEvery == 0)
            printStatus(simTime, *boidList[0], turningLeft);

        simTime += DT;
        frameCount++;
    }

    // -------------------------------------------------------------------
    // 3. Final stats
    // -------------------------------------------------------------------
    std::cout << "\n=== Simulation complete ===\n";
    std::cout << "Total frames simulated: " << frameCount << "\n";
    std::cout << "Total simulated time:   " << simTime << " s\n";

    // Print final positions of all boids
    std::cout << "\nFinal boid positions (first 10):\n";
    int printCount = std::min((int)boidList.size(), 10);
    for (int i = 0; i < printCount; i++)
    {
        glm::vec3 p = boidList[i]->getPosition();
        float altitude = glm::length(p) - GLOBE_RADIUS;
        std::cout << "  Boid " << i
                  << "  pos=(" << std::fixed << std::setprecision(2)
                  << p.x << ", " << p.y << ", " << p.z << ")"
                  << "  altitude=" << altitude
                  << "  speed=" << glm::length(boidList[i]->getVelocity()) << "\n";
    }

    // -------------------------------------------------------------------
    // 4. Clean up
    // -------------------------------------------------------------------
    for (Boids *b : boidList)
        delete b;

    return 0;
}
