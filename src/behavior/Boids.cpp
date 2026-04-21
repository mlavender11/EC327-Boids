#include "Boids.h"
using namespace std;

int Boids::nextID = 0;

Boids::Boids(float maxAlt, float minAlt)
{
    maxSpeed = 5.0f;
    maxForce = 10.0f;
    random_device rd;
    mt19937 gen(rd());

    // Uniform distribution on spherical shell between minAlt and maxAlt
    uniform_real_distribution<float> distrib_radius(minAlt, maxAlt);
    uniform_real_distribution<float> distrib_theta(0.0f, 2.0f * M_PI);
    uniform_real_distribution<float> distrib_cos_phi(-1.0f, 1.0f);

    float r = distrib_radius(gen);
    float theta = distrib_theta(gen);
    float cos_phi = distrib_cos_phi(gen);
    float sin_phi = sqrt(1.0f - cos_phi * cos_phi);

    float x = r * sin_phi * cos(theta);
    float y = r * sin_phi * sin(theta);
    float z = r * cos_phi;

    position = glm::vec3(x, y, z);

    // Random velocity components
    uniform_real_distribution<float> distrib_vel(-1.0f, 1.0f);
    glm::vec3 randomVel(distrib_vel(gen), distrib_vel(gen), distrib_vel(gen));

    // Project to tangent plane (remove radial component)
    glm::vec3 radialDir = glm::normalize(position);
    randomVel -= radialDir * glm::dot(randomVel, radialDir);

    // Random speed in [0.5, maxSpeed]. The nonzero floor is important:
    // a zero initial velocity makes align()/cohere() produce NaN on the first
    // update (normalize of a zero vector), and the NaN propagates to position.
    uniform_real_distribution<float> distrib_speed(0.5f, maxSpeed);
    float randomSpeed = distrib_speed(gen);

    // Guard against the extremely rare case where randomVel's tangent component is zero
    glm::vec3 tangentDir = (glm::length(randomVel) > 1e-5f)
                               ? glm::normalize(randomVel)
                               : glm::normalize(glm::cross(radialDir, glm::vec3(0.0f, 0.0f, 1.0f)));
    velocity = tangentDir * randomSpeed;

    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

    id = nextID++;

    direction = tangentDir;

    this->maxAlt = maxAlt;
    this->minAlt = minAlt;
}

Boids::Boids(double in_x, double in_y, double in_z)
{
    position = glm::vec3(in_x, in_y, in_z); // little fix change the second in_y -> in_z
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    maxSpeed = 3;
    maxForce = 0.5;
    id = nextID++;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distrib(0.0f, 1);
    float xDir = distrib(gen);
    float yDir = distrib(gen);
    float zDir = distrib(gen);
    direction = glm::vec3(xDir, yDir, zDir);
    direction = glm::normalize(direction);
}

Boids::Boids(double in_x, double in_y, double in_z, glm::vec3 in_vel)
{
    position = glm::vec3(in_x, in_y, in_z);
    velocity = in_vel;
    acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::normalize(velocity);
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
        // Guard: if neighbor positions cancel out, steer is ~0 and normalize() produces NaN.
        if (glm::dot(steer, steer) > 1e-10f)
        {
            steer = glm::normalize(steer);      // Average steer direction
            steer *= maxSpeed;                  // Boids want to go max speed
            steer -= velocity;                  // Vector for how to steer to get to desired
            steer = limitMagnitude(steer, maxForce);
        }
        else
        {
            steer = glm::vec3(0.0f);
        }
    }
    return steer;
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
        // Guard: if neighbor velocities sum to ~0, normalize() produces NaN that
        // propagates through update() into position and the boid renders off-screen.
        if (glm::dot(steer, steer) > 1e-10f)
        {
            steer = glm::normalize(steer);      // Avg flock direction
            steer *= maxSpeed;                  // Boid wants to go max speed
            steer -= velocity;
            steer = limitMagnitude(steer, maxForce);
        }
        else
        {
            steer = glm::vec3(0.0f);
        }
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

glm::vec3 Boids::handleBoundary()
{
    float altitude = glm::length(position);

    glm::vec3 steer(0.0f);
    float turnForce = maxForce * 2.0f; // Stronger force for boundaries

    // Check if below minimum altitude
    if (altitude < minAlt)
    {
        // Push away from center (up)
        glm::vec3 radialDirection = glm::normalize(position);
        steer = radialDirection * turnForce;
    }
    // Check if above maximum altitude
    else if (altitude > maxAlt)
    {
        // Pull toward center (down)
        glm::vec3 radialDirection = glm::normalize(position);
        steer = -radialDirection * turnForce;
    }

    return steer;
}

void Boids::flock(const vector<const Boids *> &neighbors)
{

    glm::vec3 seperate_vec = separate(neighbors);
    glm::vec3 align_vec = align(neighbors);
    glm::vec3 cohere_vec = cohere(neighbors);
    glm::vec3 boundary_vec = handleBoundary();

    // Weighting factors - maybe we want to make sliders for these
    seperate_vec *= 1.5f;
    align_vec *= 1.0f;
    cohere_vec *= 1.0f;
    boundary_vec *= 2.0f;

    applyForce(seperate_vec);
    applyForce(align_vec);
    applyForce(cohere_vec);
    applyForce(boundary_vec);
}

vector<const Boids *> Boids::findNeighbors(const vector<Boids *> &allBoids) const
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

    // Only refresh direction if velocity has meaningful magnitude.
    // normalize(0) produces NaN and would propagate through the renderer.
    if (glm::dot(velocity, velocity) > 1e-10f)
    {
        direction = glm::normalize(velocity);
    }
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

bool Boids::IsAlive() const
{
    return true;
}