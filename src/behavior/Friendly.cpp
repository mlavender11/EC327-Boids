#include "Friendly.h"

Friendly::Friendly() : Boids(){
    state = ALIVE;
}

void Friendly::Assign_Flock(Flock &flock){
    state = ALIVE;
    flock.AddFriendly(*this);
}

