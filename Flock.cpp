#include "Flock.h"

using namespace std;

Flock::Flock() { // How do we want to implement this?
    size = 0;
}

void Flock::AddBoid(Boids* new_boid){
    flock[size] = new_boid;
    size++;

}
