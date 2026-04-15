#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 vertexColor;

// Lighting Variables
uniform vec3 lightDir;
uniform vec3 ambientColor;

void main()
{
    // dFdx and dFdy find the rate of change of the 3D position of given pixels on the screen
    // The cross product of these changes is a normal vector for the surface
    // idk why the link is so long but this explained it for me
    // https://www.reddit.com/r/GraphicsProgramming/comments/1ocfdnt/what_math_knowledge_is_required_to_understand_how/?solution=ced72d77ad75ed9cced72d77ad75ed9c&js_challenge=1&token=bbbe4bf1c9a2b5160829c4be34da58612fd1aaa03eeca6d71b7989d462061278

    // Calculate the vector normal to the surface currently being drawn
    vec3 xTangent = dFdx(FragPos);
    vec3 yTangent = dFdy(FragPos);
    vec3 normal = normalize(cross(xTangent, yTangent));

    // Ambient Light (Night-time illumination)
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * ambientColor;

    // ATMOSPHERIC DAY/NIGHT CALCULATION
    vec3 normLightDir = normalize(-lightDir); 
    
    // Calculate the raw angle (-1.0 to 1.0)
    float sunAngle = dot(normal, normLightDir);
    
    // Atmospheric colors
    vec3 noonColor = vec3(1.0, 0.95, 0.9);   // Bright warm white
    vec3 sunsetColor = vec3(1.0, 0.3, 0.05); // Deep vibrant orange/red

    // smoothstep(edge0, edge1, x) creates a smooth transition to simulate Rayleigh scattering
    // If the angle is 0.0 (sunset), blendFactor is 0.0. 
    // If the angle is 0.3 or higher (mid-morning to noon), blendFactor is 1.0.
    float blendFactor = smoothstep(0.0, 0.3, sunAngle);

    // mix() blends the two colors based on the blendFactor
    vec3 dynamicLightColor = mix(sunsetColor, noonColor, blendFactor);

    // Calculate final diffuse light
    // Still use max(sunAngle, 0.0) though so the dark side of the planet doesn't glow backwards
    float diff = max(sunAngle, 0.0);
    vec3 diffuse = diff * dynamicLightColor;

    // Combine Results
    vec3 finalColor = (ambient + diffuse) * vertexColor;
    
    FragColor = vec4(finalColor, 1.0);
}