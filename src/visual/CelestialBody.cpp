#include "CelestialBody.h"

CelestialBody::CelestialBody(float radius, int sectors, int stacks)
{
    generateGeometry(radius, sectors, stacks);
    setupMesh();
}

void CelestialBody::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CelestialBody::generateGeometry(float radius, int sectors, int stacks)
{
    const float PI = 3.14159265359f;

    // Generate vertices
    for (int i = 0; i <= stacks; ++i)
    {
        float V = (float)i / (float)stacks; // Latitude
        float phi = V * PI;                 // from 0 to PI

        for (int j = 0; j <= sectors; ++j)
        {
            float U = (float)j / (float)sectors; // Longitude
            float theta = U * (PI * 2.0f);       // from 0 to 2PI

            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);

            vertices.push_back({glm::vec3(x, y, z)});
        }
    }

    // Generate indices (connect vertices into triangles)
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int first = (i * (sectors + 1)) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void CelestialBody::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers (Position is at layout location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glBindVertexArray(0);
}
