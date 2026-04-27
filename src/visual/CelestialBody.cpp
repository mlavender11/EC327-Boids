#include "CelestialBody.h"

CelestialBody::CelestialBody(float radius, int sectors, int stacks)
{
    GenerateGeometry(radius, sectors, stacks);
    SetupMesh();
}

void CelestialBody::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CelestialBody::GenerateGeometry(float radius, int sectors, int stacks)
{
    vertices.clear();
    indices.clear();

    // 1. GENERATE VERTICES
    for (int i = 0; i <= stacks; ++i)
    {
        float V = static_cast<float>(i) / static_cast<float>(stacks);
        float phi = V * glm::pi<float>();

        for (int j = 0; j <= sectors; ++j)
        {
            float U = static_cast<float>(j) / static_cast<float>(sectors);
            float theta = U * 2.0f * glm::pi<float>();

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            Vertex vertex;
            vertex.Position = glm::vec3(x * radius, y * radius, z * radius);

            // Invert U so the texture wraps correctly left-to-right
            vertex.TexCoords = glm::vec2(1.0f - U, V);

            vertices.push_back(vertex);
        }
    }

    // 2. GENERATE INDICES (This part remains exactly the same)
    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void CelestialBody::SetupMesh()
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

    // Position attribute (Layout Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute (Layout Location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
