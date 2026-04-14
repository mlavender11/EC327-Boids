#ifndef FLOCK_H
#define FLOCK_H

#include "Boids.h"

class Flock{
private: // should be made public or friend for my predator
    vector<Boids> boids; // I think that for flock we should make the vector out of Friedly or make a get func- Ilias
public:
    Flock();
    Flock(int n);

    void AddBoid(Boids& new_boid);
    void update();
};


#endif