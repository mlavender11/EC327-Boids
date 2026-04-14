#include "Flock.h"

using namespace std;

Flock::Flock() { // How do we want to implement this?
    //boids(0);
}

Flock::Flock(int n) { 
    for (int i = 0; i < n; i++){
        boids.push_back(Boids());
    }
}

void Flock::AddBoid(Boids& new_boid){
    boids.push_back(new_boid);

}

void Flock::update(){
    for (auto& boid : boids){
        boid.flock(boids);
        boid.update();
    }
}
