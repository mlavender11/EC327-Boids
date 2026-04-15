#ifndef FRIENDLY_H
#define FRIENDLY_H
#include "src/behavior/Boids.h"
#include "src/behavior/Flock.h"
#include "glm.hpp"

enum STATES{
    DEAD,
    ALIVE
};

class Friendly : public Boids{
    public: 
    int state;
    Friendly();
    void Assign_Flock(Flock &flock);
};

#endif