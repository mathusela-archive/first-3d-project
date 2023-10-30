#version 330 core

in vec3 fPos;

out vec4 FragColor;

uniform vec3 lightPos;
float farPlane = 100;

void main()
{
    float lightDistance = length(fPos - lightPos);

    lightDistance = lightDistance / farPlane;
    
    FragColor = vec4(lightDistance, lightDistance, lightDistance, 1.0);

}  