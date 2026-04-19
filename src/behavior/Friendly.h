#ifndef FRIENDLY_H
#define FRIENDLY_H
#include "src/behavior/Boids.h"
#include "glm.hpp"

class Flock;

enum class STATE{
    DEAD,
    ALIVE
};

class Friendly : public Boids{
    public: 
    STATE state;
    Friendly();
    
};

#endif