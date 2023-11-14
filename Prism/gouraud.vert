#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord;

out vec3 LightingColor;
out vec3 FragPos;
out vec3 LightPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

//uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	FragPos = vec3(view * model * vec4(aPos, 1.0f));
	LightPos = vec3(view * vec4(lightPos, 1.0f));
	Normal = mat3(transpose(inverse(view * model))) * aNormal; // transpose is a costly operation, ideally should be calculated once on CPU and passed to shader
	gl_Position = projection * vec4(FragPos, 1.0);

	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f); // greater than 90 degrees is negative so clamp to 0
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0f;
    vec3 viewDir = normalize(-FragPos); // subtract FragPos from view position which is (0,0,0) in view coordinates
    vec3 reflectDir = reflect(-lightDir, norm); // reflect expects vector from light source pointing to fragment, lightSource currently faces opposite direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    LightingColor = ambient + diffuse + specular;
}