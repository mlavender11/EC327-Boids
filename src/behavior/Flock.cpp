#include "Flock.h"

using namespace std;

Flock::Flock() {}

Flock::Flock(int n, float minAlt, float maxAlt)
{
    for (size_t i = 0; i < n; i++)
    {
        Boids *new_boid = new Friendly(maxAlt, minAlt);
        flock.push_back(new_boid);
    }
}

void Flock::Update(double dt,
                   std::vector<Predator> &predators,
                   float cohesion, float separation, float alignment,
                   float visualRange, float maxSpeed, float maxForce)
{
    vector<const Boids *> neighbors;
    neighbors.reserve(flock.size());

    for (Boids *boid : flock)
    {
        boid->setMaxSpeed(maxSpeed);
        boid->setMaxForce(maxForce);

        neighbors.clear();
        boid->findNeighbors(flock, visualRange, neighbors);
        boid->flock(neighbors, cohesion, separation, alignment, visualRange);

        // Flee from every predator
        for (Predator &p : predators)
        {
            glm::vec3 away = boid->getPosition() - p.GetPosition();
            float distSq = glm::dot(away, away);
            if (distSq < 20.0f * 20.0f && distSq > 1e-6f)
            {
                // Steer away, weighted stronger than regular flocking
                glm::vec3 steer = glm::normalize(away) * maxSpeed - boid->getVelocity();
                boid->applyForce(steer * 2.5f);
            }
        }
    }

    for (Boids *boid : flock)
        boid->update(static_cast<float>(dt));
}

size_t Flock::GetSizeOfFlock() const { return flock.size(); }

const vector<Boids *> Flock::GetFlock() const { return flock; }

Flock::~Flock()
{
    for (Boids *boid : flock) delete boid;
    flock.clear();
}

Flock::Flock(Flock &&other) noexcept : flock(std::move(other.flock)) {}

Flock &Flock::operator=(Flock &&other) noexcept
{
    if (this != &other)
    {
        for (Boids *boid : flock) delete boid;
        flock = std::move(other.flock);
    }
    return *this;
}

std::vector<Boids *> &Flock::GetFriendlies() { return flock; }