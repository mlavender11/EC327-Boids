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

glm::vec3 Boids::getVelocity() const
{
    return velocity;
}

double Boids::getDistance(const Boids &boid1, const Boids &boid2)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition());
}

void Boids::applyForce(const glm::vec3 &force)
{
    acceleration += force;
}

glm::vec3 Boids::seek(glm::vec3 target)
{ // not finished
    glm::vec3 desired = target - position;
    desired = glm::normalize(desired) * maxSpeed;
    glm::vec3 steer = desired - velocity;
    if (glm::length(steer) > maxForce)
    {
        steer = glm::normalize(steer) * maxForce;
    }

    return steer;
}



void Boids::update()
{ // not finished
    velocity += acceleration;

    if (glm::length(velocity) > maxSpeed)
        glm::normalize(velocity) * maxSpeed;
    
    position += velocity;
    acceleration = glm::vec3(0.0f,0.0f,0.0f);
    

}

void Boids::flock(const vector<Boids>& boids) {
    glm::vec3 sep = separate(boids);
    glm::vec3 ali = align(boids);
    glm::vec3 coh = cohere(boids);

    //Weighting for different forces
    sep *= 1.5f;
    ali *= 1.0f;
    coh *= 1.0f;

    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
}
