#ifndef FLOCK_H
#define FLOCK_H

#include "Friendly.h" // CMakeFiles.txt is set up to find this header file - Kyle
#include <glm/glm.hpp>
#include <cassert>
#include <vector>

class Flock
{
private:                    // should be made public or friend for my predator
    vector<Friendly> flock; // I think that for flock we should make the vector out of Friedly or make a get func- Ilias

public:
    Flock();
    Flock(int n);
    Friendly &Get_Friendly(int i);
    size_t GetSizeOfFLock() const;

    void AddFriendly(const Friendly &new_friendly);
    void update();
    const std::vector<Friendly> &GetAllFriendlies() const;
};

#endif