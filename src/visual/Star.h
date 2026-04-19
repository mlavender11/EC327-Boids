#ifndef STAR_H
#define STAR_H

#include <glm/glm.hpp>
#include "CelestialBody.h"
#include "Shader.h"

class Star
{
public:
    // Constructor sets up the size, orbit, and color
    Star(float radius, float orbitDistance, float orbitSpeed, glm::vec3 color);

    // Updates the orbital position based on elapsed time
    void Update(float time);

    // Renders the physical sphere
    void Draw(Shader &shader, const glm::mat4 &view, const glm::mat4 &projection);

    // Getters so the rest of the engine can use the light data
    glm::vec3 GetDirection() const;
    glm::vec3 GetColor() const;
    float GetOrbitDistance() const;

private:
    CelestialBody geometry;
    glm::vec3 color;

    float distance;
    float speed;

    glm::vec3 currentDirection;
    glm::vec3 currentPosition;
};

#endif