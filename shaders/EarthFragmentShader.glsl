#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightDir;
uniform vec3 ambientColor;
uniform sampler2D earthTextureMap; 

void main()
{
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

    // Grab the image color directly
    vec3 textureColor = texture(earthTextureMap, TexCoords).rgb;
    
    vec3 finalColor = (ambient + diffuse) * textureColor;
    FragColor = vec4(finalColor, 1.0);
}