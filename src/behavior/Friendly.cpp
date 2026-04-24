#include "Friendly.h"
#include <cmath>

Friendly::Friendly(glm::vec3 pos, float minAlt, float maxAlt) {
    position = pos;
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);

    maxSpeed = 3.0f;
    maxForce = 0.5f;
    minAltitude = minAlt;
    maxAltitude = maxAlt;
    alive = true;
}

void Friendly::ApplyForce(glm::vec3 force) {
    acceleration += force;
}

void Friendly::Update(float dt) {
    if (!alive) return;

    // Apply spherical-shell boundary force so the friendly stays between
    // minAltitude and maxAltitude, matching the Boids::handleBoundary() logic.
    ApplyForce(HandleBoundary());

    // FIX: scale acceleration by dt so physics is frame-rate independent.
    velocity += acceleration * dt;

    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;

    position += velocity * dt;
    acceleration = glm::vec3(0.0f);
}

void Friendly::Flee(glm::vec3 predatorPos) {
    if (!alive) return;

    glm::vec3 desired = position - predatorPos;

    // FIX: use an epsilon guard instead of exact == 0 comparison on a float.
    if (glm::length(desired) < 1e-6f) return;

    desired = glm::normalize(desired) * maxSpeed;
    glm::vec3 steer = desired - velocity;

    if (glm::length(steer) > maxForce)
        steer = glm::normalize(steer) * maxForce;

    ApplyForce(steer);
}

// FIX: Implement flocking among Friendlies using the same separate/align/cohere
// pattern as Boids.cpp, driven by the live UI slider weights.
void Friendly::FlockWith(const std::vector<Friendly>& all,
                         float weightCohesion,
                         float weightSeparation,
                         float weightAlignment,
                         float visualRange)
{
    if (!alive) return;

    glm::vec3 sepSteer(0.0f);
    glm::vec3 aliSteer(0.0f);
    glm::vec3 cohSum(0.0f);

    int sepCount = 0;
    int aliCount = 0;
    int cohCount = 0;

    float sepRange   = visualRange * 0.5f;
    float rangeSq    = visualRange * visualRange;
    float sepRangeSq = sepRange * sepRange;

    for (const Friendly& other : all) {
        if (!other.IsAlive()) continue;
        if (&other == this) continue;

        glm::vec3 delta = position - other.GetPosition();
        float distSq = glm::dot(delta, delta);

        if (distSq < 1e-10f) continue; // same position, skip

        // Separation
        if (distSq < sepRangeSq) {
            glm::vec3 away = delta / distSq; // weight by inverse distance
            sepSteer += away;
            sepCount++;
        }

        // Alignment & cohesion use the full visual range
        if (distSq < rangeSq) {
            aliSteer += other.GetVelocity();
            aliCount++;

            cohSum += other.GetPosition();
            cohCount++;
        }
    }

    // --- Separation ---
    if (sepCount > 0) {
        sepSteer /= (float)sepCount;
        float len = glm::length(sepSteer);
        if (len > 1e-6f) {
            sepSteer = (sepSteer / len) * maxSpeed - velocity;
            sepSteer = LimitMagnitude(sepSteer, maxForce);
        } else {
            sepSteer = glm::vec3(0.0f);
        }
    }

    // --- Alignment ---
    if (aliCount > 0) {
        aliSteer /= (float)aliCount;
        float len = glm::length(aliSteer);
        if (len > 1e-6f) {
            aliSteer = (aliSteer / len) * maxSpeed - velocity;
            aliSteer = LimitMagnitude(aliSteer, maxForce);
        } else {
            aliSteer = glm::vec3(0.0f);
        }
    }

    // --- Cohesion ---
    glm::vec3 cohSteer(0.0f);
    if (cohCount > 0) {
        glm::vec3 target = cohSum / (float)cohCount;
        cohSteer = Seek(target);
    }

    ApplyForce(sepSteer  * weightSeparation);
    ApplyForce(aliSteer  * weightAlignment);
    ApplyForce(cohSteer  * weightCohesion);
}

void Friendly::SetMaxSpeed(float speed) {
    maxSpeed = speed;
}

void Friendly::SetMaxForce(float force) {
    maxForce = force;
}

glm::vec3 Friendly::GetPosition() const {
    return position;
}

glm::vec3 Friendly::GetVelocity() const {
    return velocity;
}

bool Friendly::IsAlive() const {
    return alive;
}

void Friendly::Kill() {
    alive = false;
}

// --- Private helpers ---

glm::vec3 Friendly::LimitMagnitude(glm::vec3 vec, float maxMag) const {
    float lenSq = glm::dot(vec, vec);
    if (lenSq > maxMag * maxMag) {
        vec = (vec / std::sqrt(lenSq)) * maxMag;
    }
    return vec;
}

glm::vec3 Friendly::Seek(glm::vec3 target) const {
    glm::vec3 desired = target - position;
    float lenSq = glm::dot(desired, desired);
    if (lenSq < 1e-10f) return glm::vec3(0.0f);
    desired = (desired / std::sqrt(lenSq)) * maxSpeed;
    glm::vec3 steer = desired - velocity;
    return LimitMagnitude(steer, maxForce);
}

// Mirrors Boids::handleBoundary(): pushes the friendly back toward the shell
// with increasing urgency as it approaches either altitude limit.
glm::vec3 Friendly::HandleBoundary() const {
    float altitude = glm::length(position);
    if (altitude < 1e-6f) return glm::vec3(0.0f); // at origin — no radial direction

    glm::vec3 radialDir = position / altitude;

    float margin      = (maxAltitude - minAltitude) * 0.1f;
    float innerDanger = minAltitude + margin;
    float outerDanger = maxAltitude - margin;

    if (altitude < innerDanger) {
        float urgency = (innerDanger - altitude) / margin; // 0→1
        return radialDir * maxForce * 3.0f * urgency;      // push outward
    }
    if (altitude > outerDanger) {
        float urgency = (altitude - outerDanger) / margin; // 0→1
        return -radialDir * maxForce * 6.0f * urgency;     // pull inward
    }
    return glm::vec3(0.0f);
}