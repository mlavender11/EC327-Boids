#include "Boids.h"
using namespace std;

int Boids::nextID = 0;

float pi = 3.14159265358979323846f; // Hardcode because M_PI was causing issues when building on Windows - Kyle

Boids::Boids(float maxAlt, float minAlt)
{
    maxSpeed = 5.0f;
    maxForce = 10.0f;
    random_device rd;
    mt19937 gen(rd());

    // Uniform distribution on spherical shell between minAlt and maxAlt
    uniform_real_distribution<float> distrib_radius(minAlt, maxAlt);
    uniform_real_distribution<float> distrib_theta(0.0f, 2.0f * pi);
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

double Boids::distanceToSquared(const Boids &other_boid) const
{
    glm::vec3 delta = this->position - other_boid.getPosition();
    return glm::dot(delta, delta);
}

void Boids::applyForce(const glm::vec3 &force)
{
    acceleration += force;
}

glm::vec3 Boids::seek(const glm::vec3 target)
{
    glm::vec3 desired = target - position;

    float desiredLengthSq = glm::dot(desired, desired);
    if (desiredLengthSq < 0.001f * 0.001f) // If already there (squared threshold)
    {
        return glm::vec3(0.0f);
    }

    float desiredLength = sqrt(desiredLengthSq);
    desired = (desired / desiredLength) * maxSpeed;

    glm::vec3 steer = desired - velocity;
    steer = limitMagnitude(steer, maxForce);

    return steer;
}

glm::vec3 Boids::handleBoundary()
{
    float altitudeSq = glm::dot(position, position);
    float altitude = sqrt(altitudeSq);
    glm::vec3 radialDirection = position / altitude;
    glm::vec3 steer(0.0f);

    // Target altitude (centerline)
    float targetAlt = (minAlt + maxAlt) / 2.0f;

    // Gentle spring-like force toward centerline
    float displacement = altitude - targetAlt;
    steer = -radialDirection * displacement * 0.5f; // Adjust strength as needed

    // Original boundary forces (much stronger near edges)
    float margin = (maxAlt - minAlt) * 0.1f;
    float innerDanger = minAlt + margin;
    float outerDanger = maxAlt - margin;

    if (altitude < innerDanger)
    {
        float urgency = (innerDanger - altitude) / margin;
        steer += radialDirection * maxForce * 3.0f * urgency;
    }
    else if (altitude > outerDanger)
    {
        float urgency = (altitude - outerDanger) / margin;
        steer += -radialDirection * maxForce * 6.0f * urgency;
    }

    return steer;
}

// Changed - Kyle
void Boids::flock(const vector<const Boids *> &neighbors, float weightCohesion, float weightSeparation, float weightAlignment, float visualRange)
{
    glm::vec3 steer_separate(0.0f);
    glm::vec3 steer_align(0.0f);
    glm::vec3 steer_cohere(0.0f);

    int count_separate = 0;
    int count_align_cohere = 0; // Neighbors already filtered by visualRange

    // The minimum physical space a boid takes up
    float physicalBoidRadius = 5.0f; // Approximation

    // Use either half the visual range, OR the physical size, whichever is larger
    float separationDist = std::max(visualRange * 0.5f, physicalBoidRadius);
    float desiredSeparationSq = separationDist * separationDist;

    // Calculate all three behaviors
    for (const Boids *other_boid : neighbors)
    {
        // Alignment and Cohesion
        steer_align += other_boid->getVelocity();
        steer_cohere += other_boid->getPosition();
        count_align_cohere++;

        // Separation
        float distanceSq = distanceToSquared(*other_boid);
        if (distanceSq < desiredSeparationSq)
        {
            glm::vec3 diff = position - other_boid->getPosition();

            // If they're very close, give a tiny random nudge
            if (distanceSq < 0.01f)
            {
                diff = glm::vec3(0.1f, 0.1f, 0.1f);
                distanceSq = 0.01f;
            }

            diff /= distanceSq; // Weight by inverse distance
            steer_separate += diff;
            count_separate++;
        }
    }

    // Change separation
    if (count_separate > 0)
    {
        steer_separate /= static_cast<float>(count_separate);
        if (glm::dot(steer_separate, steer_separate) > 1e-10f)
        {
            steer_separate = glm::normalize(steer_separate) * maxSpeed;
            steer_separate -= velocity;

            // Multiply maxForce to give it priority
            steer_separate = limitMagnitude(steer_separate, maxForce * 3.0f);
        }
    }

    // Change alignment and cohesion
    if (count_align_cohere > 0)
    {
        // Alignment
        steer_align /= static_cast<float>(count_align_cohere);
        if (glm::dot(steer_align, steer_align) > 1e-10f)
        {
            steer_align = glm::normalize(steer_align) * maxSpeed;
            steer_align -= velocity;
            steer_align = limitMagnitude(steer_align, maxForce);
        }

        // Cohesion
        steer_cohere /= static_cast<float>(count_align_cohere);
        steer_cohere = seek(steer_cohere);
    }

    // Apply weights and forces
    steer_separate *= weightSeparation;
    steer_align *= weightAlignment;
    steer_cohere *= weightCohesion;
    glm::vec3 boundary_vec = handleBoundary() * 2.0f;

    applyForce(steer_separate);
    applyForce(steer_align);
    applyForce(steer_cohere);
    applyForce(boundary_vec);
}

// Updated to reduce memory allocations - Kyle
void Boids::findNeighbors(const vector<Boids *> &allBoids, float visualRange, vector<const Boids *> &outNeighbors) const
{
    float visualRangeSq = visualRange * visualRange;

    for (const Boids *other : allBoids)
    {
        if (other->getID() == this->id)
            continue;

        if (distanceToSquared(*other) < visualRangeSq)
        {
            outNeighbors.push_back(other);
        }
    }
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

glm::vec3 Boids::limitMagnitude(glm::vec3 &vec, float maxMag) const
{
    float magnitudeSq = glm::dot(vec, vec);
    float maxMagSq = maxMag * maxMag;

    if (magnitudeSq > maxMagSq)
    {
        float magnitude = sqrt(magnitudeSq);
        vec = (vec / magnitude) * maxMag;
    }

    return vec;
}

void Boids::setMaxSpeed(float newSpeed)
{
    maxSpeed = newSpeed;
}

void Boids::setMaxForce(float newForce)
{
    maxForce = newForce;
}