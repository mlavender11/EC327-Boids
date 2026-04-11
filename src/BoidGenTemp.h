#ifndef BOIDGENTEMP_H
#define BOIDGENTEMP_H

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// The input data I'll need to render the boids properly
// this isn't used yet but this is what will replace BoidGenTemp.cpp
// when we're actually simulating the boids every frame
struct BoidData
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 direction; // Must be a unit vector
};

// Function to generate test boid transformation matrices for testing purposes
// Returns a vector of transformation matrices for an arbitrary number of boids
// randomly positioned on the surface of a hollow sphere with arbitrary radii
std::vector<glm::mat4> GenerateTestBoids(int, float, float);

#endif