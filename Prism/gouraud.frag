#version 330 core

out vec4 FragColor;

in vec3 LightingColor;

in vec3 FragPos;
in vec3 LightPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(LightingColor * objectColor, 1.0f);
}