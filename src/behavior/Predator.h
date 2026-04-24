#ifndef PREDATOR_H
#define PREDATOR_H

#include <glm/glm.hpp>
#include <vector>
#include "Friendly.h"

class Predator {
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float maxSpeed;
    float maxForce;
    float minAltitude;
    float maxAltitude;

    glm::vec3 HandleBoundary() const;

public:
    Predator(glm::vec3 pos, float minAlt, float maxAlt);

    void Update(float dt, std::vector<Friendly>& friendlies);

    glm::vec3 GetPosition() const;
    glm::vec3 GetVelocity() const;
};

#endif