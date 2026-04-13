#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 vertexColor;

// Lighting Variables
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    // dFdx and dFdy find the rate of change of the 3D position of given pixels on the screen
    // The cross product of these changes is a normal vector for the surface
    // idk why the link is so long but this explained it for me
    // https://www.reddit.com/r/GraphicsProgramming/comments/1ocfdnt/what_math_knowledge_is_required_to_understand_how/?solution=ced72d77ad75ed9cced72d77ad75ed9c&js_challenge=1&token=bbbe4bf1c9a2b5160829c4be34da58612fd1aaa03eeca6d71b7989d462061278

    // Calculate the flat normal of the triangle currently being drawn
    vec3 xTangent = dFdx(FragPos);
    vec3 yTangent = dFdy(FragPos);
    vec3 normal = normalize(cross(xTangent, yTangent));

    // AMBIENT LIGHT (Simulates bounced light so shadows aren't pitch black)
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * ambientColor;

    // DIFFUSE LIGHT (Direct light hitting the surface)
    // We reverse the light direction because we want the vector pointing TOWARDS the light
    vec3 normLightDir = normalize(-lightDir); 
    
    // The dot product calculates the angle. If the light is hitting it dead on, it's 1.0. If it's hitting the side, it drops to 0.0.
    float diff = max(dot(normal, normLightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine results
    vec3 finalColor = (ambient + diffuse) * vertexColor;
    
    FragColor = vec4(finalColor, 1.0);
}