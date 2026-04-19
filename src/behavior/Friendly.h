#ifndef FRIENDLY_H
#define FRIENDLY_H
#include "Boids.h"
#include "Flock.h"
#include "glm/glm.hpp"

enum STATES
{
    DEAD,
    ALIVE
};

class Friendly : public Boids
{
public:
    int state;
    Friendly();
    void Assign_Flock(Flock &flock);
};

#endif