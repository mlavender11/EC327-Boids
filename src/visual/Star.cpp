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
    // Earth's axial tilt is ~23.5 degrees
    float tilt = glm::radians(23.5f);

    // Calculate a standard circular orbit in the X/Z plane,
    // but rotate the Y and Z coordinates by the tilt angle to simulate seasons
    currentDirection = glm::vec3(
        cos(time * speed),
        -sin(time * speed) * sin(tilt),
        sin(time * speed) * cos(tilt));

    currentDirection = glm::normalize(currentDirection);
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

float Star::GetOrbitDistance() const
{
    return distance;
}