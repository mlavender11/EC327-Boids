#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;      // Send position to Fragment Shader for lighting calculations
out vec3 vertexColor;

void main()
{
    // Calculate the world position of this vertex
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
    
    // Blue marble
    vertexColor = vec3(0.1, 0.4, 0.8);
}