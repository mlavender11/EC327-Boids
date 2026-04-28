#ifndef FLOCK_H
#define FLOCK_H

#include "Predator.h"
#include <glm/glm.hpp>
#include <cassert>
#include <vector>
#include "Boids.h"

class Flock
{
private:
    vector<Boids *> flock;

public:
    Flock();
    Flock(int n, float minAlt, float maxAlt);
    size_t GetSizeOfFlock() const;

    void Update(double dt,
                std::vector<Predator> &predators,
                float cohesion, float separation, float alignment,
                float visualRange, float maxSpeed, float maxForce);

    const vector<Boids *> GetFlock() const;

    ~Flock();
    Flock(const Flock &) = delete;
    Flock &operator=(const Flock &) = delete;
    Flock(Flock &&other) noexcept;
    Flock &operator=(Flock &&other) noexcept;
    std::vector<Boids *> &GetFriendlies();
};

#endif