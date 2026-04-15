#include "Flock.h"

using namespace std;

Flock::Flock() { // How do we want to implement this?
    //boids(0);
}

Flock::Flock(int n) { 
    for (int i = 0; i < n; i++){
        flock.push_back(Friendly());
    }
}

void Flock::AddFriendly(Friendly& new_friendly){
    flock.push_back(new_friendly);

}

void Flock::update(){
    for (int i = 0; i < flock.size(); i++){
        flock[i].update();
    }
}

Friendly Flock::Get_Friendly(int i){
    return flock[i];
}

int GetSizeOfFLock(){
    return flock.size();
}
