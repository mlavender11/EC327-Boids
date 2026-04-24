#ifndef FLOCK_H
#define FLOCK_H

#include <vector>
#include "Friendly.h"
#include "Predator.h"

class Flock {
private:
    std::vector<Friendly> friendlies;

public:
    void AddFriendly(Friendly f);

    // FIX: Accept behavior params from the UI sliders so they actually
    // influence each Friendly's motion every frame.
    void Update(float dt,
                std::vector<Predator>& predators,
                float weightCohesion,
                float weightSeparation,
                float weightAlignment,
                float visualRange,
                float maxSpeed,
                float maxForce);

    std::vector<Friendly>& GetFriendlies();
};

#endif