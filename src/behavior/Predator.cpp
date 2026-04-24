#include "Predator.h"

Predator::Predator(glm::vec3 pos, float minAlt, float maxAlt) {
    position = pos;
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);

    maxSpeed    = 4.0f;
    maxForce    = 0.6f;
    minAltitude = minAlt;
    maxAltitude = maxAlt;
}

glm::vec3 Predator::GetPosition() const {
    return position;
}

glm::vec3 Predator::GetVelocity() const {
    return velocity;
}

// Push the predator back toward the shell [minAltitude, maxAltitude].
// Mirrors the same logic used in Friendly::HandleBoundary().
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

void Predator::Update(float dt, std::vector<Friendly>& friendlies) {

    float closestDist = 1e9f;
    int targetIndex = -1;

    // Find closest alive friendly.
    for (int i = 0; i < (int)friendlies.size(); i++) {
        if (!friendlies[i].IsAlive()) continue;

        float dist = glm::length(friendlies[i].GetPosition() - position);

        if (dist < closestDist) {
            closestDist = dist;
            targetIndex = i;
        }
    }

    if (targetIndex != -1) {
        // Kill if within capture radius (do this before computing the steering
        // force so we don't try to normalize a near-zero vector).
        if (closestDist < 2.0f) {
            friendlies[targetIndex].Kill();
        }

        // FIX: guard against NaN when the predator is exactly on top of the
        // target. glm::normalize(zero-vector) produces NaN and poisons position.
        glm::vec3 desired = friendlies[targetIndex].GetPosition() - position;
        float desiredLen = glm::length(desired);
        if (desiredLen > 1e-6f) {
            desired = (desired / desiredLen) * maxSpeed;

            glm::vec3 steer = desired - velocity;
            if (glm::length(steer) > maxForce)
                steer = glm::normalize(steer) * maxForce;

            acceleration += steer;
        }
    }

    // Keep predator on the shell.
    acceleration += HandleBoundary();

    // FIX: scale acceleration by dt so physics is frame-rate independent.
    velocity += acceleration * dt;

    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;

    position += velocity * dt;
    acceleration = glm::vec3(0.0f);
}