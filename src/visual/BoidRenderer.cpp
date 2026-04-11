#include "BoidRenderer.h"

BoidRenderer::BoidRenderer()
{
    setupMesh();
}

void BoidRenderer::setupMesh()
{
    // Define a low-poly tetrahedron for basic boid shape
    // It points forward along the positive Z-axis
    std::vector<float> vertices = {
        // Base triangle
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.0f, 0.2f, -0.5f,
        // Left triangle
        -0.5f, 0.0f, -0.5f,
        0.0f, 0.0f, 1.0f, // Tip
        0.0f, 0.2f, -0.5f,
        // Right triangle
        0.5f, 0.0f, -0.5f,
        0.0f, 0.0f, 1.0f, // Tip
        0.0f, 0.2f, -0.5f,
        // Bottom triangle
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.0f, 0.0f, 1.0f // Tip
    };
    vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO); // Special buffer for the matrices

    glBindVertexArray(VAO);

    // --- standard vertex position setup ---
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // --- INSTANCING SETUP ---
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // We don't fill it with data yet, just allocate memory (e.g., for up to 10,000 boids)
    glBufferData(GL_ARRAY_BUFFER, 10000 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    // A mat4 is 4 vec4s. We must configure attribute locations 1, 2, 3, and 4
    std::size_t vec4Size = sizeof(glm::vec4);
    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(1 + i);
        glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(i * vec4Size));
        // Tell OpenGL to update this attribute once per instance, not once per vertex
        // Otherwise it'd take ages to load each frame
        glVertexAttribDivisor(1 + i, 1);
    }

    glBindVertexArray(0);
}

void BoidRenderer::DrawInstanced(const std::vector<glm::mat4> &instanceTransforms)
{
    if (instanceTransforms.empty())
    {
        return;
    }

    glBindVertexArray(VAO);

    // Update the instance buffer with the new matrix data for this frame
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceTransforms.size() * sizeof(glm::mat4), &instanceTransforms[0]);

    // Draw all of them in a single hardware call
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceTransforms.size());

    glBindVertexArray(0);
}