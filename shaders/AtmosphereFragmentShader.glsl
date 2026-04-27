#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;

uniform vec3 viewPos; // Where the camera is
uniform vec3 lightDir; // Where the sun is

uniform bool u_ColdWar;

void main()
{
    if (u_ColdWar) {
        // Sharp targeting grid line
        vec3 viewDir = normalize(viewPos - WorldPos);
        vec3 norm = normalize(Normal);
        float fresnel = max(0.0, 1.0 - abs(dot(norm, viewDir)));
        fresnel = pow(fresnel, 8.0);
        FragColor = vec4(1.0, 0.6, 0.0, fresnel); // Warning Amber
    }
    else {
        vec3 viewDir = normalize(viewPos - WorldPos);
        vec3 norm = normalize(Normal);
        
        // Fresnel Halo (glows at the edges, invisible in the middle)
        // dot product is 1.0 in the center, 0.0 at the edges.
        float fresnel = max(0.0, 1.0 - abs(dot(norm, viewDir)));
        fresnel = pow(fresnel, 4.0); // Make the edge sharper

        // Sun Glow (Atmosphere gets brighter on the side facing the sun)
        float sunIntensity = max(0.0, dot(norm, normalize(lightDir)));
        sunIntensity = pow(sunIntensity, 2.0); 

        // Combine them to calculate the transparency (Alpha)
        float alpha = (fresnel * 0.6) + (sunIntensity * fresnel * 0.6);
        alpha = clamp(alpha, 0.0, 1.0);
        
        // Light blue color
        vec3 atmosphereColor = vec3(0.3, 0.6, 1.0);

        FragColor = vec4(atmosphereColor, alpha);
    }
}