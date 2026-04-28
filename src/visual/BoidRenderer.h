#ifndef BOID_RENDERER_H
#define BOID_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Boids.h"
#include "behavior/Friendly.h"

class BoidRenderer
{
public:
    BoidRenderer();

    void DrawInstanced(const std::vector<glm::mat4> &);
    void DrawInstancedColored(const std::vector<glm::mat4> &transforms, glm::vec3 color); // *** NEW ***

    static std::vector<glm::mat4> BoidsToMatrices(const std::vector<Boids *> &);
    static std::vector<glm::mat4> BoidsToMatricesAlive(const std::vector<Boids *> &); // *** NEW — skips dead boids ***

private:
    unsigned int VAO, VBO, instanceVBO;
    int vertexCount;
    void setupMesh();
};

#endif