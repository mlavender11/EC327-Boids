#include "Friendly.h"
#include "Flock.h" // We can include Flock here since we only forward declared it in the header

Friendly::Friendly() : Boids()
{
    state = ALIVE;
}

void Friendly::Assign_Flock(Flock &flock)
{
    state = ALIVE;
    flock.AddFriendly(*this);
}
