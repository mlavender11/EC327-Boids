#ifndef FLOCK_H
#define FLOCK_H

#include "Boids.h"

class Flock{
private:
    vector<Boids> boids;
public:
    Flock();
    Flock(int n);

    void AddBoid(Boids& new_boid);
    void update();
};


#endif