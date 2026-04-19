#include "Flock.h"

using namespace std;

Flock::Flock() { // How do we want to implement this?
    //boids(0);
}

Flock::Flock(int n) { 
    for (size_t i = 0; i < n; i++){
        flock.push_back(Friendly());
    }
}

void Flock::AddFriendly(const Friendly& new_friendly){
    flock.push_back(new_friendly);

}

/*void Flock::update(){
    for (auto& boid : flock) {
        boid.update(flock);
    }
}
*/

Friendly& Flock::Get_Friendly(int i){
    assert(i >= 0 && i < flock.size());
    return flock[i];
}

size_t Flock::GetSizeOfFLock() const{
    return flock.size();
}

const std::vector<Friendly>& Flock::GetAllFriendlies() const{
    return flock;
}