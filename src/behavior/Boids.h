#ifndef BOIDS_H
#define BOIDS_H

#include <random>
#include <cmath>
#include "Point2D.h"
using namespace std;

const double COORDINATE_MAX; //Maximum x and y coordinate, to be used in random placement of bird during initialziation

class Boids{
private:
    Point2D position;
    Vector2D velocity; //Current velocity
    Vector2D acceleration; // Current steering force
    double maxSpeed; //Maximum speed
    double maxForce; //Maximum force that allowed per update

public:
    //Will add more parameter constructors as we add more parameters (ex coherence, speed)
    Boids(); //Initializes a bird with random position
    Boids(double in_x, double in_y); //Initializes a bird with a given position
    Boids(double in_x, double in_y, Vector2D in_vel); //Initialize a bird with goiven position and velocity

    //just want to read a boid's position don't need to copy the entire boid
    Vector2D operator-(const Boids& other_boid) const;//Vector from another boid to this boid
    static double getDistance(const Boids& boid1, const Boids& boid2); //Should we make this pointer?
    double distanceTo(const Boids& other_boid) const;

    void applyForce(const Vector2D& force);//Apply one steering force to this boid

    /*Before updating the bird's motion for each frame, we need to accumulate and calculate every force acting upon it, 
    and then apply these updates collectively via the `update`*/
    void update(); //Update this boid for every single simulation step

    void handleBoundary(double width, double length);//Keep this boid inside the boundary 

    Point2D getPosition() const;
    Vector2D getVelocity() const; 
    Vector2D getAcceleraation() const;
    double getMaxSpeed() const;
    double getMaxForce() const;
    int getID() const;
    
};
#endif