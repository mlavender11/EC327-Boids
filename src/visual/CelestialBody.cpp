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
            // Reverse the U coordinate to mirror the texture horizontally
            float U = 1.0f - ((float)j / (float)sectors);

            // Keep theta calculating normally so your 3D geometry doesn't break!
            // Note: we have to calculate theta using the original forward ratio
            float originalU = (float)j / (float)sectors;
            float theta = originalU * (PI * 2.0f);

            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);

            vertices.push_back({glm::vec3(x, y, z), glm::vec2(U, V)});
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

    // Set the vertex attribute pointers (Position is at layout location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // Set the texture attribute pointers (TexCoords is at layout location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
