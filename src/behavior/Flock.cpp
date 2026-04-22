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
        //Boids *new_boid = new Boids(maxAlt, minAlt);
        Boids *new_boid = new Friendly(maxAlt, minAlt); //try Friendly instead
        flock.push_back(new_boid);
    }
}

// void Flock::AddFriendly(const Friendly &new_friendly)
// {
//     flock.push_back(new_friendly);
// }

void Flock::Update(double dt, float cohesion, float separation, float alignment, float visualRange)
{
    vector<vector<const Boids *>> allNeighbors;
    allNeighbors.reserve(flock.size());

    for (const Boids *boid : flock)
    {
        // Pass visualRange into findNeighbors
        vector<const Boids *> neighbors = boid->findNeighbors(flock, visualRange);
        allNeighbors.push_back(neighbors);
    }

    for (size_t i = 0; i < flock.size(); i++)
    {
        // Pass all UI weights into flock
        flock[i]->flock(allNeighbors[i], cohesion, separation, alignment, visualRange);
    }

    for (Boids *boid : flock)
    {
        boid->update(dt);
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

const vector<Boids *> Flock::GetFlock() const
{
    return flock;
}

Flock::~Flock()
{
    for (Boids *boid : flock)
    {
        delete boid;
    }
    flock.clear();
}

Flock::Flock(Flock &&other) noexcept
    : flock(std::move(other.flock))
{
    // other.flock is now empty, so its destructor won't delete anything
}

Flock &Flock::operator=(Flock &&other) noexcept
{
    if (this != &other)
    {
        // Delete our current boids
        for (Boids *boid : flock)
        {
            delete boid;
        }

        // Take ownership of other's boids
        flock = std::move(other.flock);
    }
    return *this;
}
