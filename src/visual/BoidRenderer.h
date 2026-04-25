#ifndef BOID_RENDERER_H
#define BOID_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "behavior/Friendly.h"

class BoidRenderer
{
public:
    BoidRenderer();

    void DrawInstanced(const std::vector<glm::mat4>& matrices, glm::vec3 color);

private:
    unsigned int VAO, VBO, instanceVBO;
    int vertexCount;
    void setupMesh();
};

#endif