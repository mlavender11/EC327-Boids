#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 vertexColor;

uniform vec3 lightDir;
uniform vec3 ambientColor;

uniform bool u_ColdWar;

void main()
{
    if (u_ColdWar) {
        FragColor = vec4(1.0, 0.1, 0.1, 1.0); // Red
    }
    else {
        // dFdx and dFdy find the rate of change of the 3D position of given pixels on the screen
        // The cross product of these changes is a normal vector for the surface
        // idk why the link is so long but this explained it for me
        // https://www.reddit.com/r/GraphicsProgramming/comments/1ocfdnt/what_math_knowledge_is_required_to_understand_how/?solution=ced72d77ad75ed9cced72d77ad75ed9c&js_challenge=1&token=bbbe4bf1c9a2b5160829c4be34da58612fd1aaa03eeca6d71b7989d462061278

        vec3 xTangent = dFdx(FragPos);
        vec3 yTangent = dFdy(FragPos);
        vec3 normal = normalize(cross(xTangent, yTangent));

        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * ambientColor;

        vec3 normLightDir = normalize(lightDir); 
        float sunAngle = dot(normal, normLightDir);
        
        vec3 noonColor = vec3(1.0, 0.95, 0.9);   
        vec3 sunsetColor = vec3(1.0, 0.3, 0.05); 
        float blendFactor = smoothstep(-0.1, 0.3, sunAngle);
        vec3 dynamicLightColor = mix(sunsetColor, noonColor, blendFactor);
        
        float intensity = smoothstep(-0.15, 0.1, sunAngle);
        vec3 diffuse = intensity * dynamicLightColor;

        // Use the raw vertex color directly
        vec3 finalColor = (ambient + diffuse) * vertexColor;
        FragColor = vec4(finalColor, 1.0);
    }
}