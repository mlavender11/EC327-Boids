#ifndef PREDATOR_H
#define PREDATOR_H
#include "Boids.h"
#include "Flock.h"


class Predator : public Boids{
    public:
    Boids::Boids* target;
    Flock::Flock* target_flock;


    
    Predator(Flock::Flock* target_flock);
    void get_target();
    void update();


}
;
#endif