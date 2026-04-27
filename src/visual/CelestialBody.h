#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cstddef>

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class CelestialBody
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    CelestialBody(float, int, int);

    void Draw();

private:
    void GenerateGeometry(float, int, int);
    void SetupMesh();
};

#endif