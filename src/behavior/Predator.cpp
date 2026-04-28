#include "Predator.h"

Predator::Predator(glm::vec3 pos, float minAlt, float maxAlt) {
    position     = pos;
    velocity     = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);

    maxSpeed     = 4.0f;
    maxForce     = 0.6f;
    minAltitude  = minAlt;
    maxAltitude  = maxAlt;

    hunger          = 0.0f;  // starts full
    hungerRate      = 0.04f; // gets hungry over ~25 seconds
    hungerThreshold = 0.4f;  // starts hunting at 40% hunger
    huntCooldown    = 0.0f;
}

glm::vec3 Predator::GetPosition() const { return position; }
glm::vec3 Predator::GetVelocity() const { return velocity; }
float     Predator::GetHunger()   const { return hunger; }

glm::vec3 Predator::HandleBoundary() const {
    float altitude = glm::length(position);
    if (altitude < 1e-6f) return glm::vec3(0.0f);

    glm::vec3 radialDir = position / altitude;
    float margin      = (maxAltitude - minAltitude) * 0.1f;
    float innerDanger = minAltitude + margin;
    float outerDanger = maxAltitude - margin;

    if (altitude < innerDanger) {
        float urgency = (innerDanger - altitude) / margin;
        return radialDir * maxForce * 3.0f * urgency;
    }
    if (altitude > outerDanger) {
        float urgency = (altitude - outerDanger) / margin;
        return -radialDir * maxForce * 6.0f * urgency;
    }
    return glm::vec3(0.0f);
}

void Predator::Update(float dt, std::vector<Boids *> &flock) {

    // Hunger always grows over time, capped at 1
    hunger = std::min(1.0f, hunger + hungerRate * dt);

    // Cool down after a kill
    if (huntCooldown > 0.0f) {
        huntCooldown -= dt;
    }

    // Only hunt when hungry enough and not in cooldown
    bool isHungry = (hunger >= hungerThreshold) && (huntCooldown <= 0.0f);

    if (isHungry) {
        float closestDist = 1e9f;
        int   targetIndex = -1;

        for (int i = 0; i < (int)flock.size(); i++) {
            if (!flock[i]->isAlive()) continue;

            float dist = glm::length(flock[i]->getPosition() - position);
            if (dist < closestDist) {
                closestDist = dist;
                targetIndex = i;
            }
        }

        if (targetIndex != -1) {
            if (closestDist < 1.5f) {
                flock[targetIndex]->kill();
                hunger       = 0.0f;   // eating resets hunger
                huntCooldown = 5.0f;   // rest 5 seconds after a kill
            }

            // Steer toward target
            glm::vec3 desired = flock[targetIndex]->getPosition() - position;
            float desiredLen  = glm::length(desired);
            if (desiredLen > 1e-6f) {
                desired = (desired / desiredLen) * maxSpeed;
                glm::vec3 steer = desired - velocity;
                if (glm::length(steer) > maxForce)
                    steer = glm::normalize(steer) * maxForce;
                acceleration += steer;
            }
        }
    }
    // When not hungry, just cruise — no steering force added,
    // the predator naturally glides and boundary-corrects

    acceleration += HandleBoundary();
    velocity     += acceleration * dt;
    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;
    position     += velocity * dt;
    acceleration  = glm::vec3(0.0f);
}