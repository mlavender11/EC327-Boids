#include "Predator.h"

Predator::Predator(): Boids() 
{
    target_flock = NULL;
    target = NULL;
}

Predator::Predator(Flock::Flock new_target_flock): Boids() 
{
    target_flock = &new_target_flock;
    target = NULL;
}

void Predator::get_target(){
    if(target_flock == NULL || target_flock->GetSizeOfFLock() == 0) return;

    target = &(target_flock->Get_Friendly(0));

    if(target_flock->GetSizeOfFLock() == 1)return;


    for(int i = 1; i < target_flock->GetSizeOfFLock(); i++){
        if(getDistance(*target, target_flock->Get_Friendly(i)) < getDistance(*this, *target))
        target = &(target_flock->Get_Friendly(i));
    }
}