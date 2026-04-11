#version 330 core
layout (location = 0) in vec3 aPos;

// Add the Uniforms that receive data from main.cpp
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;

void main()
{
    // Multiply the matrix math in this exact order (projection * view * model * position)
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Keep your dark-red wireframe color
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
}