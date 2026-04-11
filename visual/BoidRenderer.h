#ifndef BOID_RENDERER_H
#define BOID_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class BoidRenderer
{
public:
    BoidRenderer();

    // Takes boid positioning data (in the form of a vector of transformation matrices) and draws all boids at once
    void DrawInstanced(const std::vector<glm::mat4> &instanceTransforms);

private:
    unsigned int VAO, VBO, instanceVBO;
    int vertexCount;
    void setupMesh();
};

#endif