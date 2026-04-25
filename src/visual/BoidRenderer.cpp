#include "BoidRenderer.h"
#include <glm/gtc/type_ptr.hpp>

BoidRenderer::BoidRenderer()
{
    setupMesh();
}

void BoidRenderer::setupMesh()
{
    std::vector<float> vertices = {
        // Base triangle
        -0.5f, 0.0f, -0.5f,
         0.5f, 0.0f, -0.5f,
         0.0f, 0.2f, -0.5f,
        // Left triangle
        -0.5f, 0.0f, -0.5f,
         0.0f, 0.0f,  1.0f,
         0.0f, 0.2f, -0.5f,
        // Right triangle
         0.5f, 0.0f, -0.5f,
         0.0f, 0.0f,  1.0f,
         0.0f, 0.2f, -0.5f,
        // Bottom triangle
        -0.5f, 0.0f, -0.5f,
         0.5f, 0.0f, -0.5f,
         0.0f, 0.0f,  1.0f
    };
    vertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 20000 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    std::size_t vec4Size = sizeof(glm::vec4);
    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(1 + i);
        glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
        glVertexAttribDivisor(1 + i, 1);
    }

    glBindVertexArray(0);
}

void BoidRenderer::DrawInstanced(const std::vector<glm::mat4>& matrices, glm::vec3 color)
{
    if (matrices.empty()) return;

    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    glUniform3fv(glGetUniformLocation(prog, "uColor"), 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, matrices.size() * sizeof(glm::mat4), &matrices[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, matrices.size());
    glBindVertexArray(0);
}