#version 330 core
layout (location = 0) in vec3 aPos;
// mat4 takes up 4 attribute slots (locations 1, 2, 3, and 4)
layout (location = 1) in mat4 instanceMatrix; 

uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main()
{
    // Use instanceMatrix instead of a uniform 'model' matrix
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    
    // Give the boids a bright yellow/orange color for now
    vertexColor = vec4(1.0, 0.8, 0.2, 1.0); 
}