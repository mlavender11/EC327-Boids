#ifndef FLOCK_H
#define FLOCK_H

#include "Friendly.h" // CMakeFiles.txt is set up to find this header file - Kyle
#include <glm/glm.hpp>
#include <cassert>
#include <vector>
#include "Boids.h"

class Flock
{
private: // should be made public or friend for my predator
    // vector<Friendly> flock; // I think that for flock we should make the vector out of Friedly or make a get func- Ilias
    vector<Boids *> flock;

public:
    Flock();
    Flock(int n, float minAlt, float maxAlt);
    // Friendly &Get_Friendly(int i);
    size_t GetSizeOfFlock() const;

    // void AddFriendly(const Friendly &new_friendly);
    void Update(double dt, float cohesion, float separation, float alignment, float visualRange); // Added parameters - Kyle
    // const vector<Friendly> &GetAllFriendlies() const;
    const vector<Boids *> GetFlock() const;

    ~Flock();
    // Delete copy operations
    Flock(const Flock &) = delete;
    Flock &operator=(const Flock &) = delete;

    // Add move operations
    Flock(Flock &&other) noexcept;
    Flock &operator=(Flock &&other) noexcept;
};

#endif