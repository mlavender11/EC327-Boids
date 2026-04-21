#include "Boids.h"
using namespace std;

int Boids::nextID = 0;

Boids::Boids()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distrib(0.0f, COORDINATE_MAX);
    float x = distrib(gen);
    float y = distrib(gen); // changed these 3 to be floats -Ilias
    float z = distrib(gen);

    position = glm::vec3(x, y, z);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 3.0f; // we can adjust it later
    maxForce = 0.5f;
    id = nextID++;
}

Boids::Boids(double in_x, double in_y, double in_z)
{
    position = glm::vec3(in_x, in_y, in_z); // little fix change the second in_y -> in_z
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 3;
    maxForce = 0.5;
    id = nextID++;
}

Boids::Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel)
{
    position = glm::vec3(in_x, in_y, in_z);
    velocity = in_vel;
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 2.0;
    maxForce = 0.1;
    id = nextID++;
}

glm::vec3 Boids::operator-(const Boids &other_boid)
{
    return this->position - other_boid.getPosition();
}

double Boids::getDistance(const Boids &boid1, const Boids &boid2)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition());
}

double Boids::distanceTo(const Boids &other_boid) const
{
    return glm::distance(this->position, other_boid.getPosition());
}

void Boids::applyForce(const glm::vec3 &force)
{
    acceleration += force;
}

glm::vec3 Boids::seek(const glm::vec3 target)
{
    glm::vec3 desired = target - position;

    if (glm::length(desired) < 0.001f) // If already there
    {
        return glm::vec3(0.0f);
    }

    desired = glm::normalize(desired) * maxSpeed;

    glm::vec3 steer = desired - velocity;
    steer = limitMagnitude(steer, maxForce);

    return steer;
}

glm::vec3 Boids::separate(const vector<const Boids *> &neighbors)
{
    glm::vec3 steer(0.0f);
    int count = 0;

    for (const Boids *other_boid : neighbors)
    {
        float distance = distanceTo(*other_boid);

        if (distance > 0 && distance < desiredSeperation) // is dist > 0 necesary? Would it ever be less than 0
        {
            glm::vec3 diff = position - other_boid->getPosition(); // Vector to neighobring boid
            diff = glm::normalize(diff);                           // Normalized, position to other boid

            diff /= distance; // Weight by distance

            steer += diff; // Add onto overall steering vector
            count++;
        }
    }

    if (count > 0) // If there are any neighbors to avoid
    {
        steer /= static_cast<float>(count); // Get average steer vector direction
        steer = glm::normalize(steer);      // Average steer direction
        steer *= maxSpeed;                  // Boids want to go max speed
        steer -= velocity;                  // Vector for how to steer to get to desired
        steer = limitMagnitude(steer, maxForce);
        return steer;
    }
}

glm::vec3 Boids::align(const vector<const Boids *> &neighbors)
{
    glm::vec3 steer(0.0f);
    int count = 0;

    for (const Boids *other_boid : neighbors)
    {
        float distance = distanceTo(*other_boid);
        if (distance > 0 && distance < neighborDist) // if within range
        {
            steer += other_boid->getVelocity(); // collect flock velocity
            count++;
        }
    }
    if (count > 0)
    {
        steer /= static_cast<float>(count); // Average flock velocity
        steer = glm::normalize(steer);      // Avg flock direction
        steer *= maxSpeed;                  // Boid wants to go max speed

        steer -= velocity;
        steer = limitMagnitude(steer, maxForce);
    }
    return steer;
}

glm::vec3 Boids::cohere(const vector<const Boids *> &neighbors)
{
    glm::vec3 sum(0.0f);
    int count = 0;

    for (const Boids *other_boid : neighbors)
    {
        float distance = distanceTo(*other_boid);

        if (distance > 0 && distance < neighborDist)
        {
            sum += other_boid->getPosition();
            count++;
        }
    }

    if (count > 0)
    {
        sum /= static_cast<float>(count);
        return seek(sum);
    }

    return sum;
}

void Boids::flock(const vector<const Boids *> &neighbors)
{

    glm::vec3 seperate_vec = separate(neighbors);
    glm::vec3 align_vec = align(neighbors);
    glm::vec3 cohere_vec = cohere(neighbors);

    // Weighting factors - maybe we want to make sliders for these
    seperate_vec *= 1.5f;
    align_vec *= 1.0f;
    cohere_vec *= 1.0f;

    applyForce(seperate_vec);
    applyForce(align_vec);
    applyForce(cohere_vec);
}

vector<const Boids *> Boids::findNeighbors(const vector<Boids*> &allBoids) const
{
    vector<const Boids *> neighbors;

    for (const Boids *other_boid : allBoids)
    {
        if (other_boid->getID() == this->id)
        {
            continue;
        }

        float distance = distanceTo(*other_boid); // This may be computationally expensive, a lot of square roots - could implement DistanceToSquared function
        if (distance < perceptionRadius)
        {
            neighbors.push_back(other_boid);
        }
    }
    return neighbors;
}

void Boids::update(float dt)
{
    velocity += acceleration * dt;
    velocity = limitMagnitude(velocity, maxSpeed);
    position += velocity * dt;
    acceleration = glm::vec3(0.0f);
}

// Getters

glm::vec3 Boids::getPosition() const
{
    return position;
}

glm::vec3 Boids::getVelocity() const
{
    return velocity;
}

glm::vec3 Boids::getAcceleraation() const
{
    return acceleration;
}

double Boids::getMaxSpeed() const
{
    return maxSpeed;
}

double Boids::getMaxForce() const
{
    return maxForce;
}

int Boids::getID() const
{
    return id;
}

glm::vec3 Boids::limitMagnitude(glm::vec3 vec, float maxMag) const
{
    float magnitude_squared = glm::dot(vec, vec); // dot product of self gives mag squared
    if (magnitude_squared > (maxMag * maxMag))
    {
        vec = glm::normalize(vec) * maxMag; // If magnitude is too large, return vector w/ same direction but max magnitude
    }

    return vec;
}