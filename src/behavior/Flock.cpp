#include "Flock.h"

using namespace std;

Flock::Flock()
{ // How do we want to implement this? // you could probably copy from my BoidGenTemp - Kyle
    // boids(0);
}

Flock::Flock(int n, float minAlt, float maxAlt)
{
    for (size_t i = 0; i < n; i++)
    {
        Boids* new_boid = new Boids(maxAlt, minAlt);
        flock.push_back(new_boid);
    }
}

// void Flock::AddFriendly(const Friendly &new_friendly)
// {
//     flock.push_back(new_friendly);
// }

void Flock::update(double dt){
    vector<vector<const Boids*>> allNeighbors; // List of lists of neighbors
    allNeighbors.reserve(flock.size());

    for (const Boids* boid : flock)
    {
        vector<const Boids*> neighbors = boid->findNeighbors(flock); // Get bird neighbors
        allNeighbors.push_back(neighbors);
    }

    for (size_t i = 0; i < flock.size(); i++)
    {
        flock[i]->flock(allNeighbors[i]); //Call flock function on all boids
    }

    for (Boids* boid : flock)
    {
        boid->update(dt); // Update all boids
    }
}


// Friendly &Flock::Get_Friendly(int i)
// {
//     assert(i >= 0 && i < flock.size());
//     return flock[i];
// }

size_t Flock::GetSizeOfFlock() const
{
    return flock.size();
}

// const std::vector<Friendly> &Flock::GetAllFriendlies() const
// {
//     return flock;
// }