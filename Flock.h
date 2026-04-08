#ifndef FLOCK_H
#define FLOCK_H

#include "Boids.h"

class Flock{
private:
    Boids* flock[100]; //How big do we want this? Maybe we should use stdlib vectors to make it dynamic
    int size; //Used for tracking number of boids present, not needed if we use vectors
public:
    Flock();

    void AddBoid(Boids* new_boid);
};


#endif