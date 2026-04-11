#ifndef EARTH_H
#define EARTH_H

#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
};

class Earth
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    Earth(float, int, int);

    void Draw();

private:
    void generateGeometry(float, int, int);
    void setupMesh();
};

#endif