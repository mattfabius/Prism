#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
//out vec3 LightPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0f));
	//LightPos = vec3(view * vec4(lightPos, 1.0f));
	Normal = mat3(transpose(inverse(model))) * aNormal; // transpose is a costly operation, ideally should be calculated once on CPU and passed to shader
	TexCoords = aTexCoords;
	gl_Position = projection * view * vec4(FragPos, 1.0);
}