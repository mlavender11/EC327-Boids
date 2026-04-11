#include "BoidGenTemp.h"

std::vector<glm::mat4> GenerateTestBoids(int count, float minRadius, float maxRadius)
{
    // make a vector of transformation matrices to store the position and orientation of each boid
    std::vector<glm::mat4> transforms;
    transforms.reserve(count);

    // Random number generator, apparently Mersenne Twister is much better than rand()
    std::random_device rd;
    std::mt19937 gen(rd());

    // Make distributions for the random starting states of the boids
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    std::uniform_real_distribution<float> altitudeDis(minRadius, maxRadius);
    std::uniform_real_distribution<float> dirDis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> speedDis(0.5f, 3.0f);

    // for each boid, give it its variables and add them to the transformation matrix vector
    for (int i = 0; i < count; i++)
    {
        // Random position in spherical coordinates https://en.wikipedia.org/wiki/Spherical_coordinate_system
        float r = altitudeDis(gen);
        float theta = dis(gen) * 2.0f * 3.14159f;
        float phi = acos(2.0f * dis(gen) - 1.0f);

        glm::vec3 pos;
        pos.x = r * sin(phi) * cos(theta);
        pos.y = r * cos(phi);
        pos.z = r * sin(phi) * sin(theta);

        // Generate a random direction (forward direction)
        glm::vec3 randomDir(dirDis(gen), dirDis(gen), dirDis(gen));

        // Safety check: if randomDir is (0,0,0), give it a direction to point to
        if (glm::length(randomDir) < 0.001f)
        {
            randomDir = glm::vec3(1.0f, 0.0f, 0.0f);
        }

        glm::vec3 direction = glm::normalize(randomDir);

        // Calculate Velocity = Speed * direction
        float speed = speedDis(gen);

        glm::vec3 velocity = direction * speed;

        // Set Earth's normal vector at this position to get the local Up vector for the boid
        glm::vec3 earthNormal = glm::normalize(pos);

        // Right vector (perpendicular to Earth's normal and the boid's direction)
        glm::vec3 right = glm::normalize(glm::cross(earthNormal, direction));

        // Local Up vector (perpendicular to direction and right)
        glm::vec3 localUp = glm::normalize(glm::cross(direction, right));

        // Add to the transformation matrix that will store all boids
        glm::mat4 model = glm::mat4(1.0f);
        model[0] = glm::vec4(right, 0.0f);     // X axis
        model[1] = glm::vec4(localUp, 0.0f);   // Y axis
        model[2] = glm::vec4(direction, 0.0f); // Z axis (Forward)
        model[3] = glm::vec4(pos, 1.0f);       // Translation

        model = glm::scale(model, glm::vec3(0.2f));

        transforms.push_back(model);
    }
    return transforms;
}