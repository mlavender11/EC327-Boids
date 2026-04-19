#ifndef PREDATOR_H
#define PREDATOR_H
#include "src/behavior/Boids.h"
#include "src/behavior/Friendly.h"
#include "src/behavior/Flock.h"



class Predator : public Boids {
public:
    Friendly* target;
    Flock* target_flock;

    Predator();
    Predator(Flock& new_target_flock);
    void get_target();
    void update();
};

#endif