#ifndef PREDATOR_H
#define PREDATOR_H

#include <glm/glm.hpp>
#include <vector>
#include "Boids.h"

class Predator
{
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float maxSpeed;
    float maxForce;
    float minAltitude;
    float maxAltitude;

    float hunger;          // 0 = full, 1 = starving
    float hungerRate;      // how fast hunger grows per second
    float hungerThreshold; // only hunts when hunger exceeds this
    float huntCooldown;    // pause after a kill before hunting again

    glm::vec3 HandleBoundary() const;

public:
    Predator(glm::vec3 pos, float minAlt, float maxAlt);

    void Update(float dt, std::vector<Boids *> &flock);

    glm::vec3 GetPosition() const;
    glm::vec3 GetVelocity() const;
    float GetHunger() const; // so UI or renderer can read it later

    // Setters
    void SetMaxSpeed(float speed);
    void SetMaxForce(float force);
    void SetHungerRate(float rate);
};

#endif