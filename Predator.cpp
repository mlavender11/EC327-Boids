#include "Predator.h"

Predator::Predator(): Boids() 
{
    target_flock = NULL;
    target = NULL;
}

Predator::Predator(Flock::Flock target_flock): Boids() 
{
    this.target_flock = target_flock;
    target = NULL;
}

void Predator::get_target(){
    if(target_flock == NULL || target_flock.size() == 0) return;

    target = target.flock[0];

    if(target_flock.size == 1)return;


    for(int i = 1; i < target_flock.size(); i++){
        if(distanceTo(target) > distanceTo(target_flock[i])) target = target_flock[i];
    }
}