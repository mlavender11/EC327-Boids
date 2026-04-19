#ifndef FRIENDLY_H
#define FRIENDLY_H
#include "Boids.h"
// #include "Flock.h" // This was causing a circular dependency, we can forward declare Flock instead
class Flock; // Forward declaration to avoid circular dependency
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