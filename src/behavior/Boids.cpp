#include "Boids.h"
using namespace std;

Boids::Boids() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, COORDINATE_MAX);
    double x = distrib(gen);
    double y = distrib(gen);

    position = Point2D(x, y);
    velocity = Vector2D(0.0,0.0);
    acceleration = Vector2D(0.0,0.0);
    maxSpeed = 3; // we can adjust it later
    maxForce = 0.5;
    id = 1; 


}

Boids::Boids(double in_x, double in_y) {
    position = Point2D(in_x, in_y);
    velocity = Vector2D(0.0, 0.0);
    acceleration = Vector2D(0.0,0.0);
    maxSpeed = 3;
    maxForce = 0.5;
    id = 1;
}

Boids::Boids(double in_x, double in_y, Vector2D in_vel) {
    position = Point2D(in_x, in_y);
    velocity = in_v;
    acceleration = Vector2D(0.0, 0.0);
    maxSpeed = 2.0;
    maxForce = 0.1;
    id = 1;
}

Vector2D Boids::operator-(Boids other_boid){
    return this->position - other_boid.getPosition();
}

Point2D Boids::getPosition() const
{
    return position;
}

double Boids::getDistance(Boids boid1, Boids boid2) {
    Point2D loc1 = boid1.getPosition();
    Point2D loc2 = boid2.getPosition();
    Vector2D delta = loc2 - loc1;

    double dx = delta.x;
    double dy = delta.y;

    double distance_squared = (dx * dx) + (dy * dy);

    return sqrt(distance_squared);
}
