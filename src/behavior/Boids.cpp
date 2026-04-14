#include "Boids.h"
using namespace std;

Boids::Boids()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, COORDINATE_MAX);
    double x = distrib(gen);
    double y = distrib(gen);
    double z = distrib(gen);

    position = glm::vec3(x, y, z);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 3; // we can adjust it later
    maxForce = 0.5;
    id = 1;
}

Boids::Boids(double in_x, double in_y, double in_z)
{
    position = glm::vec3(in_x, in_y, in_y);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 3;
    maxForce = 0.5;
    id = 1;
}

Boids::Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel)
{
    position = glm::vec3(in_x, in_y, in_z);
    velocity = in_vel;
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 2.0;
    maxForce = 0.1;
    id = 1;
}

glm::vec3 Boids::operator-(const Boids &other_boid)
{
    return this->position - other_boid.getPosition();
}

glm::vec3 Boids::getPosition() const
{
    return position;
}

double Boids::getDistance(const Boids &boid1, const Boids &boid2)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition());
}
