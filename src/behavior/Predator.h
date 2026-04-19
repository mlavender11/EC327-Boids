#ifndef PREDATOR_H
#define PREDATOR_H
#include "Boids.h"
#include "Friendly.h"
#include "Flock.h"

class Predator : public Boids
{
public:
    Friendly::Friendly *target;
    Flock::Flock *target_flock;

    Predator();
    Predator(Flock target_flock);
    void get_target();
    void update();
};

#endif