#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 instanceMatrix; 

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;      // Send position to Fragment Shader for lighting calculations
out vec3 vertexColor;

void main()
{
    // For boids, the instanceMatrix takes the place of the model matrix
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
    
    // Give the boids a bright orange color
    vertexColor = vec3(1.0, 0.6, 0.1); 
}