#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 WorldPos;
out vec3 Normal;

void main()
{
    WorldPos = vec3(model * vec4(aPos, 1.0));
    // Calculate the normal based on the object's geometry
    Normal = mat3(transpose(inverse(model))) * aPos;  
    
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}