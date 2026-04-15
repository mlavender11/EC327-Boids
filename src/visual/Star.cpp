#include "Star.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Initialize the CelestialBody geometry in the constructor initialization list
Star::Star(float radius, float orbitDistance, float orbitSpeed, glm::vec3 color)
    : geometry(radius, 16, 16), distance(orbitDistance), speed(orbitSpeed), color(color)
{
    currentDirection = glm::vec3(0.0f, -0.3f, 0.0f);
    currentPosition = glm::vec3(0.0f);
}

void Star::Update(float time)
{
    // Calculate the rotating direction vector
    currentDirection = glm::vec3(
        cos(time * speed),
        -0.3f,
        sin(time * speed));
    currentDirection = glm::normalize(currentDirection);

    // Calculate the physical position in space
    currentPosition = currentDirection * distance;
}

void Star::Draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Build the model matrix to translate the sphere to its current orbital position
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), currentPosition);
    shader.setMat4("model", modelMatrix);

    // Send the color to the unlit star fragment shader
    glUniform3fv(glGetUniformLocation(shader.ID, "sunColor"), 1, glm::value_ptr(color));

    geometry.Draw();
}

glm::vec3 Star::GetDirection() const
{
    return currentDirection;
}

glm::vec3 Star::GetColor() const
{
    return color;
}