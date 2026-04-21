#ifndef BOIDS_H
#define BOIDS_H

#include <random>
#include <cmath>
#include <glm/glm.hpp>
using namespace std;

// This is unnecessary bc the user defines the max (and min) altitudes for the boids in the setup menu
const double COORDINATE_MAX = 0.0; // Maximum x and y coordinate, to be used in random placement of bird during initialziation

class Boids
{
private:
    // Do we still need direction member??
    glm::vec3 position;
    glm::vec3 velocity;     // Current velocity
    glm::vec3 acceleration; // Current steering force
    float maxSpeed;         // Maximum speed
    float maxForce;         // Maximum force that allowed per update
    int id;

    // Behavior params - will need to edit based on Kyle's map
    float desiredSeperation = 25.0f; // Desired distance to maintain - used in seperate func - implemenr variation in this?
    float neighborDist = 50.0f;      // Distance for alignment and cohesion - used in aligh func
    float perceptionRadius = 50.0f; //junk value, need to fix ??

    glm::vec3 limitMagnitude(glm::vec3 vec, float maxMag) const; // Helper function for behavior calculations

    static int nextID;

public:
    // Will add more parameter constructors as we add more parameters (ex coherence, speed)
    Boids();                                                        // Initializes a bird with random position
    Boids(double in_x, double in_y, double in_z);                   // Initializes a bird with a given position
    Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel); // Initialize a bird with goiven position and velocity

    // just want to read a boid's position don't need to copy the entire boid
    glm::vec3 operator-(const Boids &other_boid);                      // Vector from another boid to this boid
    static double getDistance(const Boids &boid1, const Boids &boid2); // Static function for distance between two boids
    double distanceTo(const Boids &other_boid) const;                  // Member function for distance to another boid

    // Boids algorithm
    void applyForce(const glm::vec3 &force);                                                          // Apply one steering force to this boid
    glm::vec3 seek(const glm::vec3 target);                                                           // Head towards a position
    glm::vec3 separate(const vector<const Boids *> &neighbors);                                       // Seperate if boids are too close
    glm::vec3 align(const vector<const Boids *> &neighbors);                                          // steer towards avg velocity of neighbors
    glm::vec3 cohere(const vector<const Boids *> &neighbors);                                         // Steer toward average position of neighbors
    void flock(const vector<const Boids *> &neighbors);                                               // Calculate forces from neighboring boids in the flock
    vector<const Boids *> findNeighbors(const vector<Boids*> &allBoids) const; // Find neighbors within perception radius

    /*Before updating the bird's motion for each frame, we need to accumulate and calculate every force acting upon it,
    and then apply these updates collectively via the `update`*/
    void update(float dt); // Update this boid for every single simulation step

    void handleBoundary(double width, double length); // THIS IS FOR 2D - need to talk with kyle about 3d boundaries

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    glm::vec3 getAcceleraation() const;
    double getMaxSpeed() const;
    double getMaxForce() const;
    int getID() const;
};
#endif