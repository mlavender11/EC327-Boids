#include "Friendly.h"

Friendly::Friendly(float maxAlt, float minAlt)
    : Boids(maxAlt, minAlt), state(STATE::ALIVE)
{
}

bool Friendly::IsAlive() const
{
    return state == STATE::ALIVE;
}

void Friendly::Kill()
{
    state = STATE::DEAD;
}