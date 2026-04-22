#include "Predator.h"
#include "Flock.h"
#include <limits>
Predator::Predator(float maxAlt, float minAlt): Boids(maxAlt,minAlt), targetFlock(nullptr) 
{
//     target_flock = nullptr;
//     target = nullptr;
}

Predator::Predator(float maxAlt, float minAlt, const Flock* newTargetFlock): Boids(maxAlt, minAlt), targetFlock(newTargetFlock)
{//specify which flock the Predator should track at the time of its creation
//     target_flock = &new_target_flock;
//     target = nullptr;
}


void Predator::SetTargetFlock(const Flock *newTargetFlock)
{// modify the targetflock
    targetFlock = newTargetFlock;
}


//void Predator::get_target(){
//     if(target_flock == nullptr || target_flock->GetSizeOfFlock() == 0) return;

//     target = &target_flock->Get_Friendly(0);

//     if(target_flock->GetSizeOfFlock() == 1)return;


//     for(int i = 1; i < target_flock->GetSizeOfFlock(); i++){
//         if (getDistance(*this, target_flock->Get_Friendly(i)) < getDistance(*this, *target)){
//             target = &target_flock->Get_Friendly(i);
//             }
//     }
// }
void Predator::Hunt(float dt)
{
    if (targetFlock == nullptr || targetFlock->GetSizeOfFlock() == 0)
    //if there is no target flock has been bound yet, or if the flock is empty, do nothing
    {
        return;
    }
    const std::vector<Boids *> &allBoids = targetFlock->GetFlock();//consistently use Boids* as the container element type.
    const Boids *closestTarget = nullptr;//set null at first means havent found any targets yet
    double closestDistSq = std::numeric_limits<double>::max();

    for (const Boids *boid : allBoids)
    {
        double distSq = distanceToSquared(*boid);
        if (distSq < closestDistSq)
        {
            closestDistSq = distSq;
            closestTarget = boid;
        }
    }
    if (closestTarget != nullptr) //start chasing when found closet target
    {
        glm::vec3 chaseForce = seek(closestTarget->getPosition());// return a steering force for the target
        applyForce(chaseForce);
        update(dt);
    }
}
