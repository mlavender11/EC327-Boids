#ifndef BOIDS_H
#define BOIDS_H

#include <random>
#include "Point2D.h"
using namespace std;

const double COORDINATE_MAX; //Maximum x and y coordinate, to be used in random placement of bird during initialziation

class Boids{
private:
    Point2D position;

public:
    //Will add more parameter constructors as we add more parameters (ex coherence, speed)
    Boids(); //Initializes a bird with random position
    Boids(double in_x, double in_y); //Initializes a bird with a given position
};
#endif