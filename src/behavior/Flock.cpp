#include "Flock.h"

void Flock::AddFriendly(Friendly f) {
    friendlies.push_back(f);
}

std::vector<Friendly>& Flock::GetFriendlies() {
    return friendlies;
}

// FIX: Accept and forward the live UI slider values (speed, force, weights,
// visual range) into each Friendly so the controls actually take effect.
void Flock::Update(float dt,
                   std::vector<Predator>& predators,
                   float weightCohesion,
                   float weightSeparation,
                   float weightAlignment,
                   float visualRange,
                   float maxSpeed,
                   float maxForce)
{
    for (auto& f : friendlies) {
        if (!f.IsAlive()) continue;

        // Apply current slider values before computing forces.
        f.SetMaxSpeed(maxSpeed);
        f.SetMaxForce(maxForce);

        // Flee all predators.
        for (auto& p : predators) {
            f.Flee(p.GetPosition());
        }

        // Flock with neighbours using the live weight sliders.
        f.FlockWith(friendlies, weightCohesion, weightSeparation, weightAlignment, visualRange);

        f.Update(dt);
    }
}