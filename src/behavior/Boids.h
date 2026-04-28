#ifndef BOIDS_H
#define BOIDS_H

#include <random>
#include <cmath>
#include <glm/glm.hpp>
using namespace std;

class Boids
{
private:
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float maxSpeed;
    float maxForce;
    int id;

    glm::vec3 limitMagnitude(glm::vec3 &vec, float maxMag) const;

    static int nextID;

    float minAlt;
    float maxAlt;

protected:
    bool alive = true;  // *** NEW ***

public:
    virtual ~Boids() = default;
    Boids(float maxAlt, float minAlt);
    Boids(double in_x, double in_y, double in_z);
    Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel);

    glm::vec3 operator-(const Boids &other_boid);
    static double getDistance(const Boids &boid1, const Boids &boid2);
    double distanceTo(const Boids &other_boid) const;
    double distanceToSquared(const Boids &other_boid) const;

    void applyForce(const glm::vec3 &force);
    glm::vec3 seek(const glm::vec3 target);

    glm::vec3 separate(const vector<const Boids *> &neighbors, float visualRange);
    glm::vec3 align(const vector<const Boids *> &neighbors, float visualRange);
    glm::vec3 cohere(const vector<const Boids *> &neighbors, float visualRange);
    void flock(const vector<const Boids *> &neighbors, float weightCohesion, float weightSeparation, float weightAlignment, float visualRange);
    void findNeighbors(const vector<Boids *> &allBoids, float visualRange, vector<const Boids *> &outNeighbors) const;

    void setMaxSpeed(float newSpeed);
    void setMaxForce(float newForce);

    glm::vec3 handleBoundary();
    void update(float dt);

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    glm::vec3 getAcceleraation() const;
    double getMaxSpeed() const;
    double getMaxForce() const;
    int getID() const;

    bool isAlive() const { return alive; }  // *** NEW ***
    void kill()          { alive = false; } // *** NEW ***
};
#endif