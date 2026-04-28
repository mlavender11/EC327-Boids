#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// An instanced mat4 takes up layout locations 2, 3, 4, and 5
layout (location = 2) in mat4 instanceMatrix; 

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 vertexColor;

void main()
{
    
    // Multiply by the instanced matrix so the boids actually move
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));

    vertexColor = aColor; 
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}