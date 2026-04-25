#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 uColor;

out vec3 FragPos;
out vec3 vertexColor;

void main()
{
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    vertexColor = uColor;
}