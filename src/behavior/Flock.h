#ifndef FLOCK_H
#define FLOCK_H

#include "Friendly.h"
#include <glm/glm.hpp>

class Flock
{
private:                         // should be made public or friend for my predator
    std::vector<Friendly> flock; // I think that for flock we should make the vector out of Friedly or make a get func- Ilias

public:
    Flock();
    Flock(int n);
    Friendly Get_Friendly(int i);
    int GetSizeOfFLock();

    void AddFriendly(Friendly &new_friendly);
    void update();
    const std::vector<Friendly> &GetAllFriendlies() const;
};

#endif