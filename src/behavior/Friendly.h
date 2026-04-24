#ifndef FRIENDLY_H
#define FRIENDLY_H

#include <glm/glm.hpp>
#include <vector>

class Friendly {
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    float maxSpeed;
    float maxForce;
    float minAltitude;
    float maxAltitude;
    bool alive;

    // Internal helpers mirroring the Boids class helpers.
    glm::vec3 LimitMagnitude(glm::vec3 vec, float maxMag) const;
    glm::vec3 Seek(glm::vec3 target) const;
    glm::vec3 HandleBoundary() const;

public:
    Friendly(glm::vec3 pos, float minAlt, float maxAlt);

    void Update(float dt);
    void ApplyForce(glm::vec3 force);

    void Flee(glm::vec3 predatorPos);

    // FIX: Flocking behaviour among Friendlies, driven by the UI sliders.
    void FlockWith(const std::vector<Friendly>& all,
                   float weightCohesion,
                   float weightSeparation,
                   float weightAlignment,
                   float visualRange);

    // FIX: Setters so Flock::Update can apply live UI slider values each frame.
    void SetMaxSpeed(float speed);
    void SetMaxForce(float force);

    glm::vec3 GetPosition() const;
    glm::vec3 GetVelocity() const;
    bool IsAlive() const;
    void Kill();
};

#endif