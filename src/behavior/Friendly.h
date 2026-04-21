#ifndef FRIENDLY_H
#define FRIENDLY_H
#include "Boids.h" // CMakeFiles.txt is set up to find this header file - Kyle
#include "glm/glm.hpp"

class Flock;

enum class STATE
{
    DEAD,
    ALIVE
};

class Friendly : public Boids
{
public:
    STATE state;
    Friendly();
};

#endif