#ifndef BOIDS_H
#define BOIDS_H

#include <random>
#include <cmath>
#include <glm.hpp>
#include "Point2D.h"
using namespace std;

const double COORDINATE_MAX; //Maximum x and y coordinate, to be used in random placement of bird during initialziation

class Boids{
private:
    glm::vec3 position;
    glm::vec3 velocity; //Current velocity
    glm::vec3 acceleration; // Current steering force
    double maxSpeed; //Maximum speed
    double maxForce; //Maximum force that allowed per update
    int id;

public:
    //Will add more parameter constructors as we add more parameters (ex coherence, speed)
    Boids(); //Initializes a bird with random position
    Boids(double in_x, double in_y, double in_z); //Initializes a bird with a given position
    Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel); //Initialize a bird with goiven position and velocity

    //just want to read a boid's position don't need to copy the entire boid
    glm::vec3 operator-(const Boids& other_boid);//Vector from another boid to this boid
    static double getDistance(const Boids& boid1, const Boids& boid2); //Should we make this pointer?
    double distanceTo(const Boids& other_boid) const;

    void applyForce(const glm::vec3& force);//Apply one steering force to this boid

    /*Before updating the bird's motion for each frame, we need to accumulate and calculate every force acting upon it, 
    and then apply these updates collectively via the `update`*/
    void update(); //Update this boid for every single simulation step

    void handleBoundary(double width, double length);//Keep this boid inside the boundary 

    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const; 
    glm::vec3 getAcceleraation() const;
    double getMaxSpeed() const;
    double getMaxForce() const;
    int getID() const;
    
};
#endif