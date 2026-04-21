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
    // We don't fill it with data yet, just allocate memory.
    // Must be >= the maximum boid count exposed by the UI slider in UIManager.cpp.
    glBufferData(GL_ARRAY_BUFFER, 20000 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

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

/*
std::vector<glm::mat4> BoidRenderer::BoidsToMatrices(const std::vector<Boids> &in_boids)
{
    std::vector<glm::mat4> matrices;
    matrices.reserve(in_boids.size());

    for (const auto &boid : in_boids)
    {
        // Use the struct's data to build the orientation vectors
        glm::vec3 earthNormal = glm::normalize(boid.position);
        glm::vec3 right = glm::normalize(glm::cross(earthNormal, boid.heading));
        glm::vec3 localUp = glm::normalize(glm::cross(boid.heading, right));

        // Build the 4x4 matrix
        glm::mat4 model = glm::mat4(1.0f);
        model[0] = glm::vec4(right, 0.0f);
        model[1] = glm::vec4(localUp, 0.0f);
        model[2] = glm::vec4(boid.heading, 0.0f);
        model[3] = glm::vec4(boid.position, 1.0f);

        model = glm::scale(model, glm::vec3(0.2f));
        matrices.push_back(model);
    }
    return matrices;
}
*/

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

    // Draw all of them
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceTransforms.size());

    glBindVertexArray(0);
}

// My BoidsToMatrices class - Ilias
std::vector<glm::mat4> BoidRenderer::BoidsToMatrices(const std::vector<Boids*>& boids){
    std::vector<glm::mat4> matrices;
    matrices.reserve(boids.size());

    for (const auto &boid : boids)
    {
        glm::vec3 position = boid->getPosition();
        glm::vec3 velocity = boid->getVelocity();

        glm::vec3 direction =
            (glm::length(velocity) < 0.001f)
            ? glm::vec3(0.0f, 0.0f, 1.0f)
            : glm::normalize(velocity);

        glm::vec3 up = glm::normalize(position);
        glm::vec3 right = glm::normalize(glm::cross(up, direction));
        glm::vec3 localUp = glm::normalize(glm::cross(direction, right));

        glm::mat4 model(1.0f);
        model[0] = glm::vec4(right, 0.0f);
        model[1] = glm::vec4(localUp, 0.0f);
        model[2] = glm::vec4(direction, 0.0f);
        model[3] = glm::vec4(position, 1.0f);

        model = glm::scale(model, glm::vec3(0.2f));

        matrices.push_back(model);
    }

    return matrices;
}
