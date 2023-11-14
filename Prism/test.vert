#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord;

out vec3 LocalFragPos;
out vec3 FragPos;
out vec3 LightPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
	LocalFragPos = vec3(model * vec4(aPos, 1.0f));
	FragPos = vec3(view * model * vec4(aPos, 1.0f));
	LightPos = vec3(view * vec4(lightPos, 1.0f));
	Normal = mat3(transpose(inverse(view * model))) * aNormal; // transpose is a costly operation, ideally should be calculated once on CPU and passed to shader
	gl_Position = projection * vec4(FragPos, 1.0);
}