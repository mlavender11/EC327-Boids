#include "Boids.h"
using namespace std;

Boids::Boids() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, COORDINATE_MAX);
    double x = distrib(gen);
    double y = distrib(gen);

    position = Point2D(x, y);
    
}