#include "BoidRenderer.h"

BoidRenderer::BoidRenderer()
{
    setupMesh();
}

void BoidRenderer::setupMesh()
{
    std::vector<float> vertices = {
        -0.5f, 0.0f, -0.5f,
         0.0f, 0.4f, -0.5f,
         0.5f, 0.0f, -0.5f,

        -0.5f, 0.0f, -0.5f,
         0.0f, 0.0f,  1.0f,
         0.0f, 0.4f, -0.5f,

         0.5f, 0.0f, -0.5f,
         0.0f, 0.4f, -0.5f,
         0.0f, 0.0f,  1.0f,

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 20000 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glVertexAttrib3f(1, 0.8f, 0.7f, 0.3f); // default boid color (gold)

    std::size_t vec4Size = sizeof(glm::vec4);
    for (int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(i * vec4Size));
        glVertexAttribDivisor(2 + i, 1);
    }

    glBindVertexArray(0);
}

void BoidRenderer::DrawInstanced(const std::vector<glm::mat4> &instanceTransforms)
{
    if (instanceTransforms.empty()) return;

    glBindVertexArray(VAO);
    glVertexAttrib3f(1, 0.8f, 0.7f, 0.3f); // gold for boids

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, instanceTransforms.size() * sizeof(glm::mat4), &instanceTransforms[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, instanceTransforms.size());

    glBindVertexArray(0);
}

// *** NEW — same as DrawInstanced but sets a custom color first ***
void BoidRenderer::DrawInstancedColored(const std::vector<glm::mat4> &transforms, glm::vec3 color)
{
    if (transforms.empty()) return;

    glBindVertexArray(VAO);
    glVertexAttrib3f(1, color.r, color.g, color.b); // override color

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, transforms.size() * sizeof(glm::mat4), &transforms[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, vertexCount, transforms.size());

    glBindVertexArray(0);
}

std::vector<glm::mat4> BoidRenderer::BoidsToMatrices(const std::vector<Boids *> &boids)
{
    std::vector<glm::mat4> matrices;
    matrices.reserve(boids.size());

    for (const auto &boid : boids)
    {
        glm::vec3 position = boid->getPosition();
        glm::vec3 velocity = boid->getVelocity();

        glm::vec3 direction = (glm::length(velocity) < 0.001f)
                                  ? glm::vec3(0.0f, 0.0f, 1.0f)
                                  : glm::normalize(velocity);

        glm::vec3 up      = glm::normalize(position);
        glm::vec3 right   = glm::normalize(glm::cross(up, direction));
        glm::vec3 localUp = glm::normalize(glm::cross(direction, right));

        glm::mat4 model(1.0f);
        model[0] = glm::vec4(right,     0.0f);
        model[1] = glm::vec4(localUp,   0.0f);
        model[2] = glm::vec4(direction, 0.0f);
        model[3] = glm::vec4(position,  1.0f);
        model = glm::scale(model, glm::vec3(0.2f));
        matrices.push_back(model);
    }
    return matrices;
}

// *** NEW — only includes alive boids ***
std::vector<glm::mat4> BoidRenderer::BoidsToMatricesAlive(const std::vector<Boids *> &boids)
{
    std::vector<glm::mat4> matrices;
    for (const auto &boid : boids)
    {
        if (!boid->isAlive()) continue;  // skip dead

        glm::vec3 position = boid->getPosition();
        glm::vec3 velocity = boid->getVelocity();

        glm::vec3 direction = (glm::length(velocity) < 0.001f)
                                  ? glm::vec3(0.0f, 0.0f, 1.0f)
                                  : glm::normalize(velocity);

        glm::vec3 up      = glm::normalize(position);
        glm::vec3 right   = glm::normalize(glm::cross(up, direction));
        glm::vec3 localUp = glm::normalize(glm::cross(direction, right));

        glm::mat4 model(1.0f);
        model[0] = glm::vec4(right,     0.0f);
        model[1] = glm::vec4(localUp,   0.0f);
        model[2] = glm::vec4(direction, 0.0f);
        model[3] = glm::vec4(position,  1.0f);
        model = glm::scale(model, glm::vec3(0.2f));
        matrices.push_back(model);
    }
    return matrices;
}