#ifndef BOID_RENDERER_H
#define BOID_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "behavior/Friendly.h" // included friendly instead

// #include "Boids.h"

class BoidRenderer
{
public:
    BoidRenderer();

    // Takes a vector of Boid objects and returns a vector of their transformation matrices for the next function
    // std::vector<glm::mat4> BoidsToMatrices(const std::vector<Boids> &);

    // Takes boid positioning data (in the form of a vector of transformation matrices) and draws all boids at once
    void DrawInstanced(const std::vector<glm::mat4> &);
    static std::vector<glm::mat4> BoidsToMatrices(const std::vector<Friendly>&);


private:
    unsigned int VAO, VBO, instanceVBO;
    int vertexCount;
    void setupMesh();
};

#endif