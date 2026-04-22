#ifndef PREDATOR_H
#define PREDATOR_H
#include "Boids.h" // CMakeFiles.txt is set up to find this header file - Kyle
// #include "Friendly.h"
//#include "Flock.h"

class Flock;
class Predator : public Boids{
private:
    const Flock *targetFlock; //Points to the flock of boids currently being hunted
public:
//     Friendly *target;
//     Flock *target_flock;

    Predator(float maxAlt, float minAlt);// create a Predator without an initial target flock
    Predator(float maxAlt,float minAlt,const Flock *newTargetFlock); //bind a specific target flock directly upon creation

    //void get_target();
    //void update();
    void SetTargetFlock(const Flock *newTargetFlock);//dynamically modify the target flock to be tracked later
    void Hunt(float dt); // find nearest target and hunt per sec
};

#endif